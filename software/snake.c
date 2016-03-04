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
    const u8 food_counter_iterations = 5;
    directions move_direction;
    snake_piece* new_head_of_snake;
    food_piece* current_food = NULL;
    u8 food_counter = 0;

    snake_piece* head_of_snake = malloc_new_piece(STARTING_X_CORD, STARTING_Y_CORD);
    srand(time(NULL));

    for(;;) {
        // Check if we should spawn a pixel to eat
        // If so, spawn it
        food_counter++;
        if(food_counter == food_counter_iterations) {
            food_counter = 0;

            if(current_food == NULL) {
                current_food = generate_food_piece(head_of_snake);
            }
        }

        // Read controller
        // u8 read_controller(controller*);
        move_direction = get_snake_direction();
        if(move_direction == quit) break;

        // Move snake
        new_head_of_snake = move_snake(head_of_snake, move_direction);

        // TESTING stuff - will be removed
        if(new_head_of_snake != NULL) {
            print_cords_of_snake(new_head_of_snake);
        }
        else {
            PRINT("FAILED TO MOVE SNAKE\n");
            break;
        }

        if(current_food != NULL) {
            PRINT("FOOD X = %u, FOOD Y = %u\n", current_food->x_cord, current_food->y_cord);
        }

        head_of_snake = new_head_of_snake;

        // Check if we collided with food, ourselves, or edge of screen

    }

    free_snake(head_of_snake);
    remove_food_piece(current_food);
}

//*****************************************************************************
// Supporting functions
//*****************************************************************************
snake_piece* move_snake(snake_piece* head_of_snake, directions move_direction) {
    u8 next_x_cord, next_y_cord;

    calc_moved_x_and_y(head_of_snake, move_direction, &next_x_cord, &next_y_cord);
    if( (next_x_cord > MAX_X_CORD) || (next_y_cord > MAX_Y_CORD) ) return NULL;

    snake_piece* new_head_of_snake = insert_head_of_snake(head_of_snake,
                                                          next_x_cord,
                                                          next_y_cord);
    remove_tail_of_snake(new_head_of_snake);

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
        case quit:
            PRINT("ERROR: Can't be in quit state here\n");
            break;
    }
}

food_piece* generate_food_piece(snake_piece* head) {
    const u8 x_cord_constraint = MAX_X_CORD + 1;
    const u8 y_cord_constraint = MAX_Y_CORD + 1;
    u8 generated_x_cord, generated_y_cord;
    bool food_in_snake_body;
    food_piece* generated_food;

    for(;;) {
        generated_x_cord = generate_random_number(x_cord_constraint);
        generated_y_cord = generate_random_number(y_cord_constraint);
        food_in_snake_body = check_if_food_in_snake_body(head,
                                                         generated_x_cord,
                                                         generated_y_cord);

        // Only generate the food if the x and y cords were not in the
        // body of the snake
        if(!food_in_snake_body) {
            generated_food = create_food_piece(generated_x_cord,
                                               generated_y_cord);
            break;
        }
    }

    return generated_food;
}

bool check_if_food_in_snake_body(snake_piece* head, u8 food_x_cord, u8 food_y_cord) {
    bool food_in_snake_body = false;
    snake_piece* current_piece = head;

    while(current_piece != NULL) {
        if( (food_x_cord == current_piece->x_cord) &&
            (food_y_cord == current_piece->y_cord) ) {
            food_in_snake_body = true;
            break;
        }
        current_piece = current_piece->next;
    }

    return food_in_snake_body;
}

//****************************************
// Generates a random number between 0 and
// constraint - 1
//****************************************
u8 generate_random_number(const u8 constraint) {
    u8 random_number = (u8)(rand() % constraint);
    return random_number;
}

food_piece* create_food_piece(u8 x_cord, u8 y_cord) {
    food_piece* food = (food_piece*)malloc(sizeof(food_piece));  
    if(food == NULL) {
        PRINT("ERROR mallocing food piece\n");
        exit(1);
    }

    food->x_cord = x_cord;
    food->y_cord = y_cord;

    return food;
}

void remove_food_piece(food_piece* food_to_remove) {
    if(food_to_remove != NULL) free(food_to_remove);
}

snake_piece* insert_head_of_snake(snake_piece* current_head, u8 x_cord, u8 y_cord) {
    snake_piece* new_head = malloc_new_piece(x_cord, y_cord);
    if(new_head == NULL) {
        PRINT("Failed to create new head of snake\n");
        return NULL;
    }

    new_head->next = current_head;
    current_head->prev = new_head;
    return new_head;
}

bool remove_tail_of_snake(snake_piece* head) {
    snake_piece* current_piece = head;
    snake_piece* previous_piece, *new_tail;

    if(head == NULL) return false;

    while(current_piece != NULL) {
        previous_piece = current_piece;
        current_piece = current_piece->next;
    }

    new_tail = previous_piece->prev; 
    new_tail->next = NULL;
    remove_piece(previous_piece);
    return true;
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
    PRINT("\nEnter in a direction (w,a,s,d,q for quit):");
    scanf(" %c", &direction_char);

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
            return_direction = quit;
            break;
    }

    return return_direction;
}

void print_cords_of_snake(snake_piece* head) {
    snake_piece* current_piece = head;

    PRINT("X Cord   Y Cord\n");
    PRINT("------   ------\n");

    while(current_piece != NULL) {
        PRINT("%02u      %02u\n", current_piece->x_cord, current_piece->y_cord);
        current_piece = current_piece->next;
    }
}

