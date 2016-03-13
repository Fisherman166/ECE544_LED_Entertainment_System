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
} ball;

typedef enum {NONE, BOUNDRY, PADDLE, SCORE} collision_type;

static collision_type check_collision(ball*, paddle*, paddle*);
static bool check_paddle_collision(ball*, paddle*);
static void move_ball(ball*, paddle*, paddle*);
static short invert_velocity(short);
static void move_paddle(paddle*);
static void draw_paddle(paddle*);
static void update_screen(paddle*, paddle*);
static bool check_gametick(u32, u32);

void run_pong(u32* time_msecs) {
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
                  0};

    u32 last_msecs_updated = 0;

    for(;;) {
    	// Handle time rollover case
    	if(*time_msecs < last_msecs_updated) last_msecs_updated = *time_msecs;

    	// Update the game
    	if( check_gametick(*time_msecs, last_msecs_updated) ) {
    		last_msecs_updated = *time_msecs;
    		move_paddle(&player1);
            //move_paddle(&player2);
            move_ball(&ball1, &player1, &player2);
    		update_screen(&player1, &player2);
    	}
    }
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
    if(check_ball->x_cord >= MAX_X_CORD) collision = SCORE;
    else if(check_ball->x_cord <= 0) collision = SCORE;

    // Check for paddle collision
    if( check_ball->x_cord == player1->x_cord ) {
        if( check_paddle_collision(check_ball, player1) )
            collision = PADDLE;
    }
    if( check_ball->x_cord == player2->x_cord ) {
        if( check_paddle_collision(check_ball, player2) )
            collision = PADDLE;
    }
    return collision;
}

static bool check_paddle_collision(ball* check_ball, paddle* check_paddle) {
    bool test1 = check_ball->y_cord <= check_paddle->top_of_paddle_y_cord;
    bool test2 = check_ball->y_cord > (check_paddle->top_of_paddle_y_cord - NUM_PIXELS_IN_PADDEL);
    bool collision = test1 && test2;
    return collision;
}

static void move_ball(ball* ball1, paddle* player1, paddle* player2) {
    collision_type collision = check_collision(ball1, player1, player2);

    if(collision == BOUNDRY) {
        ball1->y_velocity = invert_velocity(ball1->y_velocity);
    }
    else if(collision == PADDLE) {
        ball1->x_velocity = invert_velocity(ball1->x_velocity);
    }
    
    ball1->x_cord += ball1->x_velocity;
    ball1->y_cord += ball1->y_velocity;
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

static void update_screen(paddle* player1_paddle, paddle* player2_paddle) {
	draw_paddle(player1_paddle);
	draw_paddle(player2_paddle);
	LEDPANEL_updatepanel();
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
