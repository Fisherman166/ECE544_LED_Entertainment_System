//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: pong.c
//
// Implements all features of the pong game.
//
//*****************************************************************************

#include "pong.h"

//*****************************************************************************
// Defines
//*****************************************************************************
#define NUM_PIXELS_IN_PADDEL 5
#define PLAYER1_STARTING_X_CORD 1
#define PLAYER2_STARTING_X_CORD (MAX_X_CORD - 1)
#define PADDLE_STARTING_Y_CORD 8
#define BALL_STARTING_X_CORD 16
#define BALL_STARTING_Y_CORD 8
#define PLAYER1_COLOR RED
#define PLAYER2_COLOR BLUE

//*****************************************************************************
// Typedefs
//*****************************************************************************
typedef enum {NONE, BOUNDRY, PADDLE_PIXEL1, PADDLE_PIXEL2, PADDLE_PIXEL3,
              PADDLE_PIXEL4, PADDLE_PIXEL5, SCORE1, SCORE2} collision_type;
typedef enum {STRAIGHT, OVER1_UP1, OVER2_UP1, OVER1_DOWN1, OVER2_DOWN1} velocities;

typedef struct {
    u16 controller_device_id;
    u8 x_cord;
    u8 top_of_paddle_y_cord;
    LED_COLORS color;
} paddle;

typedef struct {
    short x_cord;
    short y_cord;
    short x_velocity;
    short y_velocity;
    velocities velocity;
} ball;

//*****************************************************************************
// Private functions
//*****************************************************************************
static collision_type check_collision(ball*, paddle*, paddle*);
static collision_type check_paddle_collision(ball*, paddle*);
static bool move_ball(ball*, paddle*, paddle*);
static short change_velocity(short, short);
static short invert_velocity(short);
static void move_paddle(paddle*);
static void draw_paddle(paddle*);
static void update_screen(ball*, paddle*, paddle*);
static void draw_score(u8, bool);
static bool check_gametick(u32, u32);
static u8 handle_score(ball*);
static bool check_exit(u16);

//*****************************************************************************
// Global variables
//*****************************************************************************
static u8 player1_score = 0;
static u8 player2_score = 0;

//*****************************************************************************
// Input parameter is the FIT timer millisecond timestamp to enable the game
// to have a way to time when a new frame should be drawn.
//*****************************************************************************
u8 run_pong(u32* time_msecs) {
    paddle player1 = {CONTROLLER1_DEV_ID,
                      PLAYER1_STARTING_X_CORD,
                      PADDLE_STARTING_Y_CORD,
                      PLAYER1_COLOR};
    paddle player2 = {CONTROLLER2_DEV_ID,
                      PLAYER2_STARTING_X_CORD,
                      PADDLE_STARTING_Y_CORD,
                      PLAYER2_COLOR};
    ball ball1 = {BALL_STARTING_X_CORD,
                  BALL_STARTING_Y_CORD,
                  1,
                  0,
                  STRAIGHT};

    u32 last_msecs_updated = 0;
    bool player_scored = false;
    u8 game_status = 0;

    // Reset the global variables so that multiple runs of the game are possible
    player1_score = 0;
    player2_score = 0;

    for(;;) {
    	// Handle timer rollover case
    	if(*time_msecs < last_msecs_updated) last_msecs_updated = *time_msecs;

    	// Update the game if a gametick has passed
    	if( check_gametick(*time_msecs, last_msecs_updated) ) {
    		last_msecs_updated = *time_msecs;
            
            // Check if player 1 wants to exit the game
            if( check_exit(player1.controller_device_id) ) break;

            // Handle the paddle and ball movement
    		move_paddle(&player1);
            move_paddle(&player2);
            player_scored = move_ball(&ball1, &player1, &player2);
            
            // Check if a player scored a point and update the screen
            if(player_scored) {
                game_status = handle_score(&ball1);
                player_scored = false;
            }
    		update_screen(&ball1, &player1, &player2);

    		// Someone won so exit the game
    		if(game_status != 0) break;
    	}
    }

    return game_status;
}

//*****************************************************************************
// Private Functions
//*****************************************************************************

//*****************************************************************************
// This function checks to see if the ball has hit either the goal area, the
// top of the screen, or one of the two paddles.
//
// This function assumes player1 is the paddle on the left side of the screen
// and player2 is the paddle on the right side of the screen.
//*****************************************************************************
static collision_type check_collision(ball* check_ball, paddle* player1,
                                      paddle* player2) {
    collision_type collision = NONE;

    // Check for hitting the top of the LED panel
    if(check_ball->y_cord >= MAX_Y_CORD) collision = BOUNDRY;
    else if(check_ball->y_cord <= 0) collision = BOUNDRY;

    // Check for hitting sides of the LED panel
    if(check_ball->x_cord >= MAX_X_CORD) collision = SCORE1;
    else if(check_ball->x_cord <= 0) collision = SCORE2;

    // Check for the ball hitting the square just before the paddle if x_velocty
    // is 1 or -1. If x_vecolity is 2 or -2, then we need to check both the pixel
    // right in front of the paddle and 1 more pixel out. If both of these
    // checks are not done, then the ball can fly through the paddle
    // because the x_velocty is fast enough to skip the pixel right in front
    // of the paddle
    if( check_ball->x_velocity == -2) {
    	if( check_ball->x_cord == (player1->x_cord + 2) ) {
    	    collision = check_paddle_collision(check_ball, player1);
    	} else if ( check_ball->x_cord == (player1->x_cord + 1) ) {
    	    collision = check_paddle_collision(check_ball, player1);
    	}
    }
    else if( check_ball->x_velocity == -1) {
    	if( check_ball->x_cord == (player1->x_cord + 1) ) {
    	    collision = check_paddle_collision(check_ball, player1);
    	}
    }
    else if( check_ball->x_velocity == 2) {
    	if( check_ball->x_cord == (player2->x_cord - 2) ) {
    	    collision = check_paddle_collision(check_ball, player2);
    	}
    	else if( check_ball->x_cord == (player2->x_cord - 1) ) {
    	    collision = check_paddle_collision(check_ball, player2);
    	}
    }
    else {
    	if( check_ball->x_cord == (player2->x_cord - 1) ) {
    	    collision = check_paddle_collision(check_ball, player2);
    	}
    }

    return collision;
}

//*****************************************************************************
// Detects which pixel of the paddle the ball has hit. Assumes that the
// paddles are both 5 pixels long. Detection is done by comparing the ball
// y value to the paddle pixels y value. This function should be called from
// a function that has already confirmed that the ball x value is in front of
// one of the paddles.
//*****************************************************************************
static collision_type check_paddle_collision(ball* check_ball, paddle* check_paddle) {
    collision_type collision = NONE;

    // Pixel1 is the top pixel of the paddle.
    // For pixel1 we need to check the y value just in front of the pixel and 1 y
    // value above it to handle balls coming in at an angle.
    // Pixel5 is the same concept, only we check 1 y value below the pixel
    // All other pixels only check the single y value of the pixel
    bool pixel1 = (check_ball->y_cord == check_paddle->top_of_paddle_y_cord) ||
    			  (check_ball->y_cord == (check_paddle->top_of_paddle_y_cord + 1));
    bool pixel2 = (check_ball->y_cord == (check_paddle->top_of_paddle_y_cord - 1));
    bool pixel3 = (check_ball->y_cord == (check_paddle->top_of_paddle_y_cord - 2));
    bool pixel4 = (check_ball->y_cord == (check_paddle->top_of_paddle_y_cord - 3));
    bool pixel5 = (check_ball->y_cord == (check_paddle->top_of_paddle_y_cord - 4)) ||
    			  (check_ball->y_cord == (check_paddle->top_of_paddle_y_cord - 5));

    if(pixel1) collision = PADDLE_PIXEL1;
    else if(pixel2) collision = PADDLE_PIXEL2;
    else if(pixel3) collision = PADDLE_PIXEL3;
    else if(pixel4) collision = PADDLE_PIXEL4;
    else if(pixel5) collision = PADDLE_PIXEL5;

    return collision;
}

//*****************************************************************************
// This function moves the ball by first checking if a collision has occurred
// between the ball and something else on the screen. If a collision has occurred,
// we handle it in this function. If no collision has occurred, then we just
// move the ball by the currently x and y velocities for the ball.
//*****************************************************************************
static bool move_ball(ball* ball1, paddle* player1, paddle* player2) {
    collision_type collision = check_collision(ball1, player1, player2);
    
    // Checks if either player scored
    bool scored = (collision == SCORE1) || (collision == SCORE2);
    bool hit_paddle = ( (collision == PADDLE_PIXEL1) ||
                        (collision == PADDLE_PIXEL2) ||
                        (collision == PADDLE_PIXEL3) ||
                        (collision == PADDLE_PIXEL4) ||
                        (collision == PADDLE_PIXEL5) );

    // BOUNDRY = top of the screen. If we hit the top of the screen,
    // then we only need to invert the y velocity of ball
    if(collision == BOUNDRY) {
        ball1->y_velocity = invert_velocity(ball1->y_velocity);
    }
    else if(hit_paddle) {
        // Handle all of the angle calculations based on how the ball is
        // currently moving and what pixel the ball hits on the paddle.
        // This changes the x and y velocities of the ball
        if(ball1->velocity == STRAIGHT) {
            switch(collision) {
                case PADDLE_PIXEL1:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL2:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER1_UP1;
                    break;
                case PADDLE_PIXEL3:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 0;
                    ball1->velocity = STRAIGHT;
                    break;
                case PADDLE_PIXEL4:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = change_velocity(ball1->y_velocity, 1);
                    ball1->velocity = OVER1_UP1;
                    break;
                case PADDLE_PIXEL5:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = change_velocity(ball1->y_velocity, 1);
                    ball1->velocity = OVER2_UP1;
                    break;
            }
        }
        else if(ball1->velocity == OVER1_UP1) {
            switch(collision) {
                case PADDLE_PIXEL1:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL2:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL3:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER1_UP1;
                    break;
                case PADDLE_PIXEL4:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 0;
                    ball1->velocity = STRAIGHT;
                    break;
                case PADDLE_PIXEL5:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER1_DOWN1;
                    break;
            }
        }
        else if(ball1->velocity == OVER1_DOWN1) {
            switch(collision) {
                case PADDLE_PIXEL1:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL2:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 0;
                    ball1->velocity = STRAIGHT;
                    break;
                case PADDLE_PIXEL3:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER2_DOWN1;
                    break;
                case PADDLE_PIXEL4:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER2_DOWN1;
                    break;
                case PADDLE_PIXEL5:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER1_DOWN1;
                    break;
            }
        }
        else if(ball1->velocity == OVER2_UP1) {
            switch(collision) {
                case PADDLE_PIXEL1:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER1_UP1;
                    break;
                case PADDLE_PIXEL2:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL3:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 0;
                    ball1->velocity = STRAIGHT;
                    break;
                case PADDLE_PIXEL4:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER1_DOWN1;
                    break;
                case PADDLE_PIXEL5:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER2_DOWN1;
                    break;
            }
        }
        else if(ball1->velocity == OVER2_DOWN1) {
            switch(collision) {
                case PADDLE_PIXEL1:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL2:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = 1;
                    ball1->velocity = OVER2_UP1;
                    break;
                case PADDLE_PIXEL3:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = 0;
                    ball1->velocity = STRAIGHT;
                    break;
                case PADDLE_PIXEL4:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 2);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER2_DOWN1;
                    break;
                case PADDLE_PIXEL5:
                    ball1->x_velocity = change_velocity(ball1->x_velocity, 1);
                    ball1->y_velocity = -1;
                    ball1->velocity = OVER1_DOWN1;
                    break;
            }
        }
    }

    // The x and y velocities can be positive or negative, so addition handles
    // all movement cases for the ball
    ball1->x_cord += ball1->x_velocity;
    ball1->y_cord += ball1->y_velocity;
    return scored;
}

// New velocity is a positive velocity number
// Current velocity is used to see if the new velocity should be positive
// or negative.
//*****************************************************************************
// Calculates what the new velocity should be based off of the desired
// velocity and the current velocity of the ball in either the x or y direction.
// This function should be used any time the balls new velocity is dependant
// on what part of the screen it hit. For example, if the ball hits the left
// paddle, then the new x velocity should be positive. If the ball hits the
// right paddle, then the new x velocity should be negative.
//
// New velocity is assumed to be a positive number. The new velocity will be
// made negative if the currentl velocity of the ball is positive. The new
// velocity will be left positive if the current velocity is negative.
//*****************************************************************************
static short change_velocity(short current_velocity, short new_velocity) {
    short resulting_velocity;

    // Check if the current velocity is positive
    if( current_velocity > 0 ) {
        resulting_velocity = invert_velocity(new_velocity);
    }
    else {
        resulting_velocity = new_velocity;
    }
    return resulting_velocity;
}

//*****************************************************************************
// A simple function that takes in a velocity and inverts it. In this case,
// by inversion we mean switching the sign (either positive or negative).
//*****************************************************************************
static short invert_velocity(short velocity) {
    short inverted_velocity;

    switch(velocity) {
        case -2:
            inverted_velocity = 2;
            break;
        case -1:
            inverted_velocity = 1;
            break;
        case 1:
            inverted_velocity = -1;
            break;
        case 2:
            inverted_velocity = -2;
            break;
        default:
            xil_printf("-W-: Got weird velocity %d\n", velocity);
            inverted_velocity = 0;
            break;
    }
    return inverted_velocity;
}

//*****************************************************************************
// Checks if the user wants to move their paddle and then moves the paddle
//*****************************************************************************
static void move_paddle(paddle* paddle_to_move) {
    // The subtraction of 1 in the min top of paddle accounts for the fact that
    // the minimum y coordinate is 0 and not 1
    const u8 max_top_of_paddle_cord = MAX_Y_CORD;
    const u8 min_top_of_paddle_cord = NUM_PIXELS_IN_PADDEL - 1;
    buttons controller = read_controller(paddle_to_move->controller_device_id); 
    u8 calculated_y_cord;

    // Move the paddle up if the up button is pressed, or move down if the
    // down button is pressed
    if(controller.up) {
        calculated_y_cord = paddle_to_move->top_of_paddle_y_cord + 1;
        if(calculated_y_cord <= max_top_of_paddle_cord)
            paddle_to_move->top_of_paddle_y_cord = calculated_y_cord;
    }
    else if(controller.down) {
        calculated_y_cord = paddle_to_move->top_of_paddle_y_cord - 1;
        if(calculated_y_cord >= min_top_of_paddle_cord)
        	paddle_to_move->top_of_paddle_y_cord = calculated_y_cord;
    }
}

//*****************************************************************************
// Since the paddle only keeps track of it's top y coordinate, this function
// needs some extra logic to draw out the other four pixels of the paddle.
//*****************************************************************************
static void draw_paddle(paddle* paddle_to_draw) {
    const u8 top_y = paddle_to_draw->top_of_paddle_y_cord;
    const int bottom_y = (int)(top_y - NUM_PIXELS_IN_PADDEL); // This goes negative at minimum y cord
    int current_y;

    // Draw the four pixels below the top pixel
    for(current_y = top_y; current_y > bottom_y; current_y--) {
        LEDPANEL_writepixel(paddle_to_draw->x_cord,
                            current_y,
                            paddle_to_draw->color);
    }
}

//*****************************************************************************
// Draw the two paddles, the ball and the players scores to the screen
//*****************************************************************************
static void update_screen(ball* ball1, paddle* player1_paddle, paddle* player2_paddle) {
    bool player1 = true;
    bool player2 = false;

	draw_paddle(player1_paddle);
	draw_paddle(player2_paddle);
	draw_score(player1_score, player1);
	draw_score(player2_score, player2);
	// Draw the ball
    LEDPANEL_writepixel(ball1->x_cord,
                        ball1->y_cord,
                        WHITE);
	LEDPANEL_updatepanel();
}

//*****************************************************************************
// Draws the scores on the screen as tallies that count up. The total number of
// pixels on siginifies the current score of the player. Scores are drawn at
// the bottom of the screen on each half the screen
//*****************************************************************************
static void draw_score(u8 score, bool player1) {
	u8 i;
	u8 x_cord;
	LED_COLORS color;

	if(player1) {
		x_cord = 5;
		color = PLAYER1_COLOR;
	}
	else {
		x_cord = 23;
		color = PLAYER2_COLOR;
	}

    // Draw all of the pixels (score) that the player currently has
	for(i = 0; i < score; i++) {
		LEDPANEL_writepixel(x_cord + i,
			                MAX_Y_CORD,
			                color);
	}
}

//*****************************************************************************
// Returns true if one gametick has passed
//*****************************************************************************
static bool check_gametick(u32 current_msecs, u32 last_msecs_updated) {
    const u32 msecs_between_update = 200;
    bool retval = false; 

    if( (current_msecs - last_msecs_updated) > msecs_between_update ) retval = true;

    return retval;
}

//*****************************************************************************
// 
//*****************************************************************************
static u8 handle_score(ball* ball1) {
	const u8 max_score = 7;
	u8 game_status = 0;
	bool player1_scored = false;

    // Increment the players score
    if(ball1->x_cord >= MAX_X_CORD) {
    	player1_score++;
    	player1_scored = true;
    }
    else player2_score++;

    // Check if someone has won the game
    if(player1_score > max_score) game_status = PLAYER1_WINS;
    else if(player2_score > max_score) game_status = PLAYER2_WINS;

    // Reset the ball so that it moves towards the player
    // that did not score on the last round.
    // Always start with the ball in the STRAIGHT velocity
    ball1->x_cord = BALL_STARTING_X_CORD;
    ball1->y_cord = BALL_STARTING_Y_CORD;
    if(player1_scored) ball1->x_velocity = 1;
    else ball1->x_velocity = -1;
    ball1->y_velocity = 0;
    ball1->velocity = STRAIGHT;

    return game_status;
}

//*****************************************************************************
// Used to check if the user has pressed the select button to exit the game
//*****************************************************************************
static bool check_exit(u16 controller_device_id) {
    buttons controller = read_controller(controller_device_id);
    bool exit = false;

    if(controller.select) exit = true;
    return exit;
}

