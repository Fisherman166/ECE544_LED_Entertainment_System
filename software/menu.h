//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: menu.h
//
//*****************************************************************************

#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include "vivado_includes.h"
#include "stdbool.h"
#include "controller.h"


#define CHOOSE_NUM   0
#define SNAKE_NUM  1
#define PONG_NUM   2
#define DRAW_NUM   3


//*****************
// Functions
//*****************
u8 select_game(u32*);

#endif
