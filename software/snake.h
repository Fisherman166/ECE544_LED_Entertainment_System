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
#include <stdio.h>
#include "vivado_includes.h"
#include "stdbool.h"
#include "controller.h"

#define PRINT xil_printf

typedef struct snake_piece snake_piece;

struct snake_piece {
    u8 x_cord;
    u8 y_cord;
    snake_piece* next;
    snake_piece* prev;
};

typedef struct  {
    u8 x_cord;
    u8 y_cord;
} food_piece;

typedef enum {up, down, left, right} movement_directions;

//*****************
// Functions
//*****************
void run_snake(u32*);

snake_piece* move_snake(snake_piece*, food_piece**, u8, u8);
snake_piece* normal_move_snake(snake_piece*, u8, u8);
snake_piece* got_food_move_snake(snake_piece*, u8, u8);
void calc_moved_x_and_y(snake_piece*, movement_directions, u8*, u8*);
bool check_snake_collision(snake_piece*, u8, u8);

food_piece* generate_food_piece(snake_piece*);
u8 generate_random_number(const u8);
food_piece* create_food_piece(u8, u8);
void remove_food_piece(food_piece**);

snake_piece* insert_head_of_snake(snake_piece*, u8, u8);
bool remove_tail_of_snake(snake_piece*);
void free_snake(snake_piece*);

snake_piece* create_snake_piece(u8, u8);
bool remove_snake_piece(snake_piece*);

movement_directions direction_to_move();

void update_screen(snake_piece*, food_piece*);

#endif

