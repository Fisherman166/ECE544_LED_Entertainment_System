//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: main.c
//
// All of the glue logic is implemented here. Device initialization, FIT timer
// interrupt handling, game selection and game over screens are handled in
// this file.
//
//*****************************************************************************

#include "main.h"
#include "snake.h"
#include "menu.h"
#include "draw.h"
#include "pong.h"
#include "game_end.h"

// IP instances
XIntc interrupt_controller;

// Generic variables
volatile u32 timestamp_msecs = 0;

int main() {
    XStatus status;

    u8 current_game = CHOOSE_NUM;
    u8 pong_winner;

    status = init_devices();
    if(status != XST_SUCCESS) PRINT("FAILED TO INIT DEVICES\n");
    else {
      // Spin until a user selects a game
        for(;;) {
            current_game = select_game(&timestamp_msecs);

            // Snake only draws the game over screen after the game ends.
            // Draw has no game end screen.
            // Pong will show which player wins the game.
            if(current_game == SNAKE_NUM) {
                run_snake(&timestamp_msecs);
                draw_game_end(GAME_OVER_NUM);
            }
            else if(current_game == DRAW_NUM) {
                run_draw(&timestamp_msecs);
            }
            else if(current_game == PONG_NUM) {
                pong_winner = run_pong(&timestamp_msecs);
                if (pong_winner == PLAYER1_WINS) draw_game_end(RED_WINS_NUM);
                else draw_game_end(BLUE_WINS_NUM);
            }
        }
    }
    
    return 0;
}

static XStatus init_devices() {
    XStatus status;

    init_platform();

    // Initialize the NES controllers
    status = NES_initialize(CONTROLLER1_DEV_ID, CONTROLLER1_BASE_ADDRESS);
    if(status != XST_SUCCESS) return XST_FAILURE;

    status = NES_initialize(CONTROLLER2_DEV_ID, CONTROLLER2_BASE_ADDRESS);
    if(status != XST_SUCCESS) return XST_FAILURE;

    //Initialize the LED panel
    status = LEDPANEL_initialize(LEDPANEL_BASE_ADDRESS);
    if(status != XST_SUCCESS) return XST_FAILURE;

    status = XIntc_Initialize(&interrupt_controller, INTC_DEVICE_ID);
    if(status != XST_SUCCESS) return XST_FAILURE;

    status = XIntc_Connect(&interrupt_controller, FIT_INTERRUPT_ID,
                           (XInterruptHandler)FIT_handler,
                           (void *)0);
    if(status != XST_SUCCESS) return XST_FAILURE;

    // Enable the global interrupt for the interrupt controller
    status = XIntc_Start(&interrupt_controller, XIN_REAL_MODE);
    if(status != XST_SUCCESS) return XST_FAILURE;

    // Enable the FIT interrupt specifically
    XIntc_Enable(&interrupt_controller, FIT_INTERRUPT_ID);

    microblaze_enable_interrupts();

    return XST_SUCCESS;
}

// Updates a timestamp every msec
void FIT_handler() {
    static u8 ts_interval = 0;

    ts_interval++;
    if(ts_interval > FIT_COUNT_1MSEC) {
        timestamp_msecs++;
        ts_interval = 1;
    }
}

