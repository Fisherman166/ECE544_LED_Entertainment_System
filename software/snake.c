//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: snake.c
//
//*****************************************************************************

#include "snake.h"


#define STARTING_X_CORD 7
#define STARTING_Y_CORD 15
#define MAX_X_CORD 15
#define MAX_Y_CORD 31

void run_snake() {
    directions move_direction;
    bool snake_moved_succusfully;

    snake_piece* head_of_snake = malloc_new_piece(STARTING_X_CORD, STARTING_Y_CORD);
    snake_piece* tail_of_snake = NULL;

    for(;;) {
        // Check if we should spawn a pixel to eat
        // If so, spawn it

        // Read controller
        // u8 read_controller(controller*);
        move_direction = get_snake_direction();

        // Move snake
        snake_moved_succusfully = move_snake(head_of_snake, move_direction);

        if(snake_moved_succusfully) {
            print_cords_of_snake(head_of_snake);
        }
        else {
            PRINT("FAILED TO MOVE SNAKE\n");
            exit(2);
        }

        // Check if we collided with a pixel, ourselves, or edge of screen

    }

    free_snake(head_of_snake);
}

//*****************************************************************************
// Supporting functions
//*****************************************************************************
snake_piece* move_snake(snake_piece* head_of_snake, directions move_direction) {
    u8 next_x_cord, next_y_cord;

    calc_moved_x_and_y(head_of_snake, move_direction, &next_x_cord, &next_y_cord);
    if( (next_x_cord > MAX_X_CORD) || (next_y_cord > MAX_X_CORD) ) return NULL;

    snake_piece* new_head_of_snake = insert_head_of_snake(head_of_snake,
                                                          next_x_cord,
                                                          next_y_cord);
    return new_head_of_snake;
}

void calc_moved_x_and_y(snake_piece* head, directions direction_to_move,
                        u8* calculated_x_cord, u8* calculated_y_cord) {
    switch(direction_to_move) {
        case up:
            *calculated_y_cord = head->y_cord + 1;
            *calculated_x_cord = head->x_cord;
            break;
        case down:
            *calculated_y_cord = head->y_cord - 1;
            *calculated_x_cord = head->x_cord;
            break;
        case left:
            *calculated_y_cord = head->y_cord;
            *calculated_x_cord = head->x_cord - 1;
            break;
        case right:
            *calculated_y_cord = head->y_cord;
            *calculated_x_cord = head->x_cord + 1;
            break;
    }
}

snake_piece* insert_head_of_snake(snake_piece* current_head, u8 x_cord, u8 y_cord) {
    snake_piece* new_head = malloc_new_piece(x_cord, y_cord);
    if(new_head == NULL) return NULL;

    new_head->next = current_head;
    current_head->prev = new_head;
    return new_head;
}

snake_piece* remove_tail_of_snake(snake_piece* current_tail) {
    if(current_tail == NULL) return NULL;

    if(current_tail->prev == NULL) return NULL;
    snake_piece* new_tail = current_tail->prev;
    new_tail->next = NULL;  // We are freeing what this points to

    bool removed = remove_piece(current_tail);
    if(removed) return new_tail;
    else return NULL;
}

void free_snake(snake_piece* head) {
    snake_piece* current_piece = head;
    snake_piece* next_piece;

    while(current_piece != NULL) {
        next_piece = current_piece->next;
        remove_piece(current_piece);
        current_piece = next_piece;
    }
}

snake_piece* malloc_new_piece(u8 x_cord, u8 y_cord) {
    snake_piece* new_piece = (snake_piece*)malloc(sizeof(snake_piece));
    if(new_piece == NULL) {
        PRINT("FAILED TO MALLOC SNAKE PIECE\n");
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

// Test functions that will be removed
directions get_snake_direction() {
    directions return_direction;
    char direction_char;
    PRINT("Enter in a direction (w,a,s,d,q for quit)\n");
    scanf("%c", &direction_char);

    switch(direction_char) {
        case 'w':
           return_direction = up;
           break;
        case 'a':
           return_direction = left;
           break;
        case 's':
           return_direction = down;
           break;
        case 'd':
           return_direction = right;
           break;
        default:
            exit(1);
            break;
    }

    return return_direction;
}

void print_cords_of_snake(snake_piece* head) {
    snake_piece* current_piece = head;

    PRINT("X Cord   Y Cord\n");
    PRINT("------   ------");

    while(current_piece != NULL) {
        PRINT("%02u      %02u\n", current_piece->x_cord, current_piece->y_cord);
        current_piece = current_piece->next;
    }
}

