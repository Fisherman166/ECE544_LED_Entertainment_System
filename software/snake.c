//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: snake.c
//
//*****************************************************************************

#include "snake.h"

void run_snake() {
    snake_piece* head_of_snake = malloc_new_piece(0,0);

snake_piece* insert_head_of_snake(snake_piece* current_head, u8 x_cord, u8 y_cord) {
    snake_piece* new_head = malloc_new_piece(x_cord, y_cord);
    if(new_head == NULL) return NULL;

    new_head->next = current_head;
    current_head->prev = new_head;
    return new_head;
}

snake_piece* malloc_new_piece(u8 x_cord, u8 y_cord) {
    snake_piece* new_piece = (snake_piece*)malloc(sizeof(snake_piece));
    if(snake_piece == NULL) {
        xil_printf("FAILED TO MALLOC SNAKE PIECE\n");
        return NULL;
    }

    new_piece->x_cord = x_cord;
    new_piece->y_cord = y_cord;
    new_piece->next = NULL;
    new_piece->prev = NULL;

    return new_piece;
}

bool remove_piece(snake_piece* node) {
    if(node == NULL) return 0;

    free(node);
    return 1;
}

