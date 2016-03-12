//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: controller.h
//
//*****************************************************************************

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "vivado_includes.h"
#include "stdbool.h"

typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    bool A;
    bool B;
    bool start;
    bool select;
} buttons;

buttons read_controller(u16);

#endif // CONTROLLER_H

