//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: main.c
//
//*****************************************************************************

#include "main.h"
#include "snake.h"

// IP instances
XIntc interrupt_controller;
XGpio controller1_gpio;

// Generic variables
volatile u32 timestamp_msecs = 0;

int main() {

    run_snake();
    
    return 0;
}

static XStatus init_devices() {
    XStatus status;

    init_platform();

    u32 controller1_input_mask = 0xFF;  // Bottom 8 bit as input
    status = XGpio_Initialize(&controller1_gpio, CONTROLLER1_DEVICE_ID);
    if(status != XST_SUCCESS) return XST_FAILURE;
    XGpio_SetDataDirection(&controller1_gpio, CONTROLLER1_INPUT_CHANNEL, controller1_input_mask);

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

