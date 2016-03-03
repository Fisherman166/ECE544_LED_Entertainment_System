//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: snake.h
//
//*****************************************************************************

#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>

typedef struct snake_piece snake_piece;

struct snake_piece {
    u8 x_cord;
    u8 y_cord;
    snake_piece* next;
    snake_piece* prev;
};

//*****************
// Functions
//*****************
void run_snake();
snake_piece* insert_head_of_snake(snake_piece*, u8, u8);

snake_piece* malloc_new_piece(u8, u8);
bool remove_piece(snake_piece*);

#endif

