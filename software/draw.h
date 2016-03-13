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
typedef enum {off, blue, green, teal, red, purple, yellow, white} color;


/**
* run_draw
* runs the draw game
*/
void run_draw();


/**
* do_move_pencil
* Changes the location that the user is currently coloring
* @param - none
* @return - boolean whether or not a change has occurred
*/
bool do_move_pencil();


/**
* do_color_pencil
* Changes the color that the user is currently drawing
* @param - bool new_loc: indicates if the location has changed
* @return - none
*/
void do_color_pencil(bool new_loc);


/**
* do_write_panel
* writes the entire array and the current pencil location to the panel
* @param - none
* @return - none
*/
void do_write_panel();
