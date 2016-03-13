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

typedef struct {
    u16 controller_device_id;
    u8 x_cord;
    u8 top_of_paddle_y_cord;
    LED_COLORS color;
} paddle;

static void move_paddle(paddle*);
static void draw_paddle(paddle*);
static bool check_gametick(u32, u32);

void run_pong(u32* time_msecs) {
    paddle player1 = {CONTROLLER1_DEV_ID,
                       1,
                       8,
                       BLUE};
    paddle player2 = {CONTROLLER2_DEV_ID,
                       14,
                       8,
                       RED};
    u32 last_msecs_updated = 0;

    for(;;) {
    	// Handle time rollover case
    	if(*time_msecs < last_msecs_updated) last_msecs_updated = *time_msecs;

    	// Update the game
    	if( check_gametick(*time_msecs, last_msecs_updated) ) {
    		last_msecs_updated = *time_msecs;
    		move_paddle(&player1);
    		draw_paddle(&player1);
    		draw_paddle(&player2);
    	}
    }
}

//*****************************************************************************
// Private Functions
//*****************************************************************************

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
        xil_printf("UP Y CORD: %d\n", calculated_y_cord);
        if(calculated_y_cord <= max_top_of_paddle_cord)
            paddle_to_move->top_of_paddle_y_cord = calculated_y_cord;
    }
    else if(controller.down) {
        calculated_y_cord = paddle_to_move->top_of_paddle_y_cord - 1;
        xil_printf("DOWN Y CORD: %d\n",calculated_y_cord);
        if(calculated_y_cord >= min_top_of_paddle_cord)
        	paddle_to_move->top_of_paddle_y_cord = calculated_y_cord;
    }
}

static void draw_paddle(paddle* paddle_to_draw) {
    u8 top_y = paddle_to_draw->top_of_paddle_y_cord;
    u8 bottom_y = top_y - NUM_PIXELS_IN_PADDEL;
    u8 current_y;

    for(current_y = top_y; current_y > bottom_y; current_y--) {
        LEDPANEL_writepixel(paddle_to_draw->x_cord,
                            current_y,
                            paddle_to_draw->color);
    }
    LEDPANEL_updatepanel();
}
 
//*****************************************************************************
// Returns true if one gametick has passed
//*****************************************************************************
static bool check_gametick(u32 current_msecs, u32 last_msecs_updated) {
    const u32 msecs_between_update = 500;
    bool retval = false; 

    if( (current_msecs - last_msecs_updated) > msecs_between_update ) retval = true;

    return retval;
}
