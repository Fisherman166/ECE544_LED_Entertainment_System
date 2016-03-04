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

#ifdef VIVADO
    #define PRINT xil_printf
#else
    #define PRINT printf
    #include <stdio.h>
    #include <unistd.h>
    #include <stdint.h>
    #include <stdbool.h>

    typedef uint8_t u8;
#endif

typedef struct snake_piece snake_piece;

struct snake_piece {
    u8 x_cord;
    u8 y_cord;
    snake_piece* next;
    snake_piece* prev;
};
typedef enum {up, down, left, right} directions;

//*****************
// Functions
//*****************
void run_snake();

snake_piece* move_snake(snake_piece*, directions);
void calc_moved_x_and_y(snake_piece*, directions, u8*, u8*);

snake_piece* insert_head_of_snake(snake_piece*, u8, u8);
snake_piece* remove_tail_of_snake(snake_piece*);
void free_snake(snake_piece*);

snake_piece* malloc_new_piece(u8, u8);
bool remove_piece(snake_piece*);

// Testing functions only - will be removed
directions get_snake_direction();
void print_cords_of_snake(snake_piece*);


#endif

