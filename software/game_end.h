//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: game_end.h
//
//*****************************************************************************

#ifndef GAME_END_H
#define GAME_END_H

#include <stdlib.h>
#include <stdio.h>
#include "vivado_includes.h"
#include "stdbool.h"
#include "controller.h"


#define GAME_OVER_NUM 0
#define BLUE_WINS_NUM 1
#define RED_WINS_NUM 2


//*****************
// Functions
//*****************
void draw_game_end(u8);

#endif
