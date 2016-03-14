//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: pong.c
//
//*****************************************************************************

#include "pong.h"

#define NUM_PIXELS_IN_PADDEL 5
#define PLAYER1_STARTING_X_CORD 1
#define PLAYER2_STARTING_X_CORD (MAX_X_CORD - 1)
#define PADDLE_STARTING_Y_CORD 8
#define BALL_STARTING_X_CORD 16
#define BALL_STARTING_Y_CORD 8


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
static bool positive_number(short);
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

u8 run_pong(u32* time_msecs) {
    paddle player1 = {CONTROLLER1_DEV_ID,
                      PLAYER1_STARTING_X_CORD,
                      PADDLE_STARTING_Y_CORD,
                      RED};
    paddle player2 = {CONTROLLER2_DEV_ID,
                      PLAYER2_STARTING_X_CORD,
                      PADDLE_STARTING_Y_CORD,
                      BLUE};
    ball ball1 = {BALL_STARTING_X_CORD,
                  BALL_STARTING_Y_CORD,
                  1,
                  0,
                  STRAIGHT};

    u32 last_msecs_updated = 0;
    bool player_scored = false;
    u8 game_status = 0;

    // Reset the global variables
    player1_score = 0;
    player2_score = 0;

    for(;;) {
    	// Handle time rollover case
    	if(*time_msecs < last_msecs_updated) last_msecs_updated = *time_msecs;

    	// Update the game
    	if( check_gametick(*time_msecs, last_msecs_updated) ) {
    		last_msecs_updated = *time_msecs;
            
            if( check_exit(player1.controller_device_id) ) break;

    		move_paddle(&player1);
            move_paddle(&player2);
            player_scored = move_ball(&ball1, &player1, &player2);
            
            if(player_scored) {
                game_status = handle_score(&ball1);
                player_scored = false;
            }
    		update_screen(&ball1, &player1, &player2);

    		// Someone won so exit game
    		if(game_status != 0) break;
    	}
    }

    return game_status;
}

//*****************************************************************************
// Private Functions
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

    // Check for the ball hitting the square just before the paddle
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

static collision_type check_paddle_collision(ball* check_ball, paddle* check_paddle) {
    collision_type collision = NONE;

    // Starting with the top pixel of the paddle
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

static bool move_ball(ball* ball1, paddle* player1, paddle* player2) {
    collision_type collision = check_collision(ball1, player1, player2);
    bool scored = (collision == SCORE1) || (collision == SCORE2);
    bool hit_paddle = ( (collision == PADDLE_PIXEL1) ||
                        (collision == PADDLE_PIXEL2) ||
                        (collision == PADDLE_PIXEL3) ||
                        (collision == PADDLE_PIXEL4) ||
                        (collision == PADDLE_PIXEL5) );

    if(collision == BOUNDRY) {
        ball1->y_velocity = invert_velocity(ball1->y_velocity);
    }
    else if(hit_paddle) {
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
    
    ball1->x_cord += ball1->x_velocity;
    ball1->y_cord += ball1->y_velocity;
    return scored;
}

// New velocity is a positive velocity number
// Current velocity is used to see if the new velocity should be positive
// or negative.
static short change_velocity(short current_velocity, short new_velocity) {
    short resulting_velocity;

    if( positive_number(current_velocity) ) {
        resulting_velocity = invert_velocity(new_velocity);
    }
    else {
        resulting_velocity = new_velocity;
    }
    return resulting_velocity;
}

static bool positive_number(short number) {
    u16 negative_bit_mask = 0x8000;
    bool positive_number = true;
    if(number < 0) positive_number = false;
    return positive_number;
}

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
    // The addition and subtraction of 1 account for the fact that the top y
    // pixel is included in the NUM_PIXELS_IN_PADDEL value.
    const u8 max_top_of_paddle_cord = MAX_Y_CORD;
    const u8 min_top_of_paddle_cord = NUM_PIXELS_IN_PADDEL - 1;
    buttons controller = read_controller(paddle_to_move->controller_device_id); 
    u8 calculated_y_cord;

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

static void draw_paddle(paddle* paddle_to_draw) {
    const u8 top_y = paddle_to_draw->top_of_paddle_y_cord;
    const int bottom_y = (int)(top_y - NUM_PIXELS_IN_PADDEL); // This goes negative at minimum y cord
    int current_y;

    for(current_y = top_y; current_y > bottom_y; current_y--) {
        LEDPANEL_writepixel(paddle_to_draw->x_cord,
                            current_y,
                            paddle_to_draw->color);
    }
}

static void update_screen(ball* ball1, paddle* player1_paddle, paddle* player2_paddle) {
	draw_paddle(player1_paddle);
	draw_paddle(player2_paddle);
	draw_score(player1_score, true);
	draw_score(player2_score, false);
	// Draw the ball
    LEDPANEL_writepixel(ball1->x_cord,
                        ball1->y_cord,
                        WHITE);
	LEDPANEL_updatepanel();
}

static void draw_score(u8 score, bool player1) {
	u8 i;
	u8 x_cord;
	LED_COLORS color;

	if(player1) {
		x_cord = 5;
		color = RED;
	}
	else {
		x_cord = 23;
		color = BLUE;
	}

	for(i = 0; i < score; i++) {
		LEDPANEL_writepixel(x_cord + i,
			                15,
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

static u8 handle_score(ball* ball1) {
	const u8 max_score = 7;
	u8 game_status = 0;
	bool player1_scored = false;
    if(ball1->x_cord >= MAX_X_CORD) {
    	player1_score++;
    	player1_scored = true;
    }
    else player2_score++;

    if(player1_score > max_score) game_status = PLAYER1_WINS;
    else if(player2_score > max_score) game_status = PLAYER2_WINS;

    // Reset the ball
    ball1->x_cord = BALL_STARTING_X_CORD;
    ball1->y_cord = BALL_STARTING_Y_CORD;
    if(player1_scored) ball1->x_velocity = 1;
    else ball1->x_velocity = -1;
    ball1->y_velocity = 0;

    return game_status;
}

static bool check_exit(u16 controller_device_id) {
    buttons controller = read_controller(controller_device_id);
    bool exit = false;

    if(controller.select) exit = true;
    return exit;
}

