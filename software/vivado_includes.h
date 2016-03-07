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
#include "xtmrctr.h"
#include "uart.h"
#include "nesip.h"

// NES controller1 defines
#define CONTROLLER1_DEV_ID 0
#define CONTROLLER1_BASE_ADDRESS 0

// Interrupt controller parameters
#define INTC_DEVICE_ID          XPAR_INTC_0_DEVICE_ID
#define FIT_INTERRUPT_ID        XPAR_MICROBLAZE_0_AXI_INTC_FIT_TIMER_0_INTERRUPT_INTR

// Fixed Interval timer - 100 MHz input clock, 5KHz output clock
#define FIT_IN_CLOCK_FREQ_HZ    CPU_CLOCK_FREQ_HZ
#define FIT_CLOCK_FREQ_HZ       5000
#define FIT_COUNT_1MSEC         5       // 1msec / (1 / 5KHz)

#endif

