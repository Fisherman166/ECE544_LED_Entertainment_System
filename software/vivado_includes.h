//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: vivado_includes.h
//
//*****************************************************************************

#ifndef VIVADO_INCLUDES_H
#define VIVADO_INCLUDES_H

#include "xparameters.h"
#include "xintc.h"
#include "xuartlite.h"
#include "nesip.h"
#include "ledpanel.h"
#include "xil_types.h"
#include "platform.h"
#include "mb_interface.h"

// NES controller1 defines
#define CONTROLLER1_DEV_ID XPAR_NESIP_0_DEVICE_ID
#define CONTROLLER1_BASE_ADDRESS XPAR_NESIP_0_NES_BASEADDR

// NES controller2 defines
#define CONTROLLER2_DEV_ID XPAR_NESIP_1_DEVICE_ID
#define CONTROLLER2_BASE_ADDRESS XPAR_NESIP_1_NES_BASEADDR

// ledpanel defines
#define LEDPANEL_BASE_ADDRESS XPAR_LEDPANEL_0_LEDPANEL_BASEADDR

// Interrupt controller parameters
#define INTC_DEVICE_ID          XPAR_INTC_0_DEVICE_ID
#define FIT_INTERRUPT_ID        XPAR_MICROBLAZE_0_AXI_INTC_FIT_TIMER_0_INTERRUPT_INTR

// Fixed Interval timer - 100 MHz input clock, 5KHz output clock
#define FIT_IN_CLOCK_FREQ_HZ    CPU_CLOCK_FREQ_HZ
#define FIT_CLOCK_FREQ_HZ       5000
#define FIT_COUNT_1MSEC         5       // 1msec / (1 / 5KHz)

typedef enum {
	OFF = 0x0,
	BLUE = 0x1,
	GREEN = 0x2,
	TEAL = 0x3,
	RED = 0x4,
	PURPLE = 0x5,
	YELLOW = 0x6,
	WHITE = 0x7
} LED_COLORS;

#endif

