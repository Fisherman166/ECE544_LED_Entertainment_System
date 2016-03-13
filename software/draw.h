//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: draw.h
//
//*****************************************************************************


#include <stdlib.h>
#include <stdio.h>
#include "vivado_includes.h"
#include "stdbool.h"
#include "controller.h"
#include "ledpanel.h"

#define DRAW_MIN_X      0       //minimum x index
#define DRAW_MAX_X      31      //maximum x index
#define DRAW_MIN_Y      0       //minimum y index
#define DRAW_MAX_Y      15      //maximum y index

//enum for colors of the led panel 3'b RGB
typedef enum {off, blue, green, teal, red, purple, yellow, white, move} color;


/**
* run_draw
* runs the draw game
*/
void run_draw(u32* timestamp_msecs);


