//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: snake.c
//
//*****************************************************************************

#include "snake.h"

#define STARTING_X_CORD 15
#define STARTING_Y_CORD 7
#define MAX_X_CORD 31
#define MAX_Y_CORD 15

void run_snake(u32* timestamp_msecs) {
    movement_directions movement_direction;
    snake_piece* new_head_of_snake;
    u8 next_x_cord, next_y_cord;
    food_piece* food = NULL;
    bool moving_head_into_body = false;
    u32 screen_updated_at_msecs = 0;

    snake_piece* head_of_snake = create_snake_piece(STARTING_X_CORD, STARTING_Y_CORD);
    srand(*timestamp_msecs);	// This will always be different

    for(;;) {
    	// Handle the timestamp rollover case
    	if( *timestamp_msecs < screen_updated_at_msecs )  screen_updated_at_msecs = *timestamp_msecs;

        // Update the state of the game on a gametick
        if( check_gametick(*timestamp_msecs, screen_updated_at_msecs) ) {
            screen_updated_at_msecs = *timestamp_msecs;

            // Always spawn food right after the last one was eaten
            if(food == NULL) {
            	food = generate_food_piece(head_of_snake);
            }

            // Read controller
            movement_direction = direction_to_move();

            // Move snake
            calc_moved_x_and_y(head_of_snake, movement_direction, &next_x_cord, &next_y_cord);
            moving_head_into_body = check_snake_collision(head_of_snake, next_x_cord, next_y_cord);
            if(moving_head_into_body) break;
            new_head_of_snake = move_snake(head_of_snake, &food, next_x_cord, next_y_cord);

            // Update the screen
            if(new_head_of_snake != NULL) {
                head_of_snake = new_head_of_snake;
                update_screen(head_of_snake, food);
            }
            else {
                PRINT("Head of snake is NULL. FAILED");
                break;
            }
        }
    } // for

    free_snake(head_of_snake);
    remove_food_piece(&food);
}

//*****************************************************************************
// Supporting functions
//*****************************************************************************
snake_piece* move_snake(snake_piece* head, food_piece** food, u8 x_cord, u8 y_cord) {
    snake_piece* new_head;

    if(x_cord > MAX_X_CORD) new_head = NULL;
    else if(y_cord > MAX_Y_CORD) new_head = NULL;
    else if(*food == NULL) {
        new_head = normal_move_snake(head, x_cord, y_cord);
    }
    else {
        if( ((*food)->x_cord == x_cord) && ((*food)->y_cord == y_cord) ) {
            new_head = got_food_move_snake(head, x_cord, y_cord);
            remove_food_piece(food);
        }
        else new_head = normal_move_snake(head, x_cord, y_cord);
    }

    return new_head;
}

snake_piece* normal_move_snake(snake_piece* head_of_snake, u8 x_cord, u8 y_cord) {
    snake_piece* new_head_of_snake = insert_head_of_snake(head_of_snake,
                                                          x_cord,
                                                          y_cord);
    remove_tail_of_snake(new_head_of_snake);

    return new_head_of_snake;
}

snake_piece* got_food_move_snake(snake_piece* head_of_snake, u8 x_cord, u8 y_cord) {

    snake_piece* new_head_of_snake = insert_head_of_snake(head_of_snake,
                                                          x_cord,
                                                          y_cord);
    return new_head_of_snake;
}

void calc_moved_x_and_y(snake_piece* head, movement_directions direction_to_move,
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

food_piece* generate_food_piece(snake_piece* head) {
    const u8 x_cord_constraint = MAX_X_CORD + 1;
    const u8 y_cord_constraint = MAX_Y_CORD + 1;
    u8 generated_x_cord, generated_y_cord;
    bool food_in_snake_body;
    food_piece* generated_food;

    for(;;) {
        generated_x_cord = generate_random_number(x_cord_constraint);
        generated_y_cord = generate_random_number(y_cord_constraint);
        food_in_snake_body = check_snake_collision(head,
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

bool check_snake_collision(snake_piece* head_of_snake, u8 x_cord_to_check, u8 y_cord_to_check) {
    bool object_in_snake = false;
    snake_piece* current_piece = head_of_snake;

    while(current_piece != NULL) {
        if( (x_cord_to_check == current_piece->x_cord) &&
            (y_cord_to_check == current_piece->y_cord) ) {
            object_in_snake = true;
            break;
        }
        current_piece = current_piece->next;
    }

    return object_in_snake;
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

void remove_food_piece(food_piece** food_to_remove) {
    if(*food_to_remove == NULL) return;
    free(*food_to_remove);
    *food_to_remove = NULL;
}

snake_piece* insert_head_of_snake(snake_piece* current_head, u8 x_cord, u8 y_cord) {
    snake_piece* new_head = create_snake_piece(x_cord, y_cord);
    if(new_head == NULL) {
        PRINT("Failed to create new head of snake\n");
        return NULL;
    }

    new_head->next = current_head;
    current_head->prev = new_head;
    current_head->color = GREEN; //All snake chunks are green but the head
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
    remove_snake_piece(previous_piece);
    return true;
}

//*********************************************************
// Free's the entire snake for cleanup
//*********************************************************
void free_snake(snake_piece* head) {
    snake_piece* current_piece = head;
    snake_piece* next_piece;

    while(current_piece != NULL) {
        next_piece = current_piece->next;
        remove_snake_piece(current_piece);
        current_piece = next_piece;
    }
}

snake_piece* create_snake_piece(u8 x_cord, u8 y_cord) {
    snake_piece* new_piece = (snake_piece*)malloc(sizeof(snake_piece));
    if(new_piece == NULL) {
        PRINT("FAILED TO MALLOC SNAKE PIECE\n");
        return NULL;
    }

    new_piece->x_cord = x_cord;
    new_piece->y_cord = y_cord;
    new_piece->color = BLUE; //Head of snake is always blue
    new_piece->next = NULL;
    new_piece->prev = NULL;

    return new_piece;
}

bool remove_snake_piece(snake_piece* node) {
    if(node == NULL) return 0;

    free(node);
    return 1;
}

movement_directions direction_to_move() {
    static movement_directions direction_currently_moving = right;
    buttons buttons_pressed = read_controller(CONTROLLER1_DEV_ID);
    movement_directions final_direction_to_move;

    // If the direction asked for by the button press tries to move back into
    // the body of the snake, we need to block that action. Instead, the snake
    // will keep moving the same direction as it was before the button press
    if(buttons_pressed.up) {
        if(direction_currently_moving == down) {
        	final_direction_to_move = direction_currently_moving;
        }
        else final_direction_to_move = up;
    }
    else if(buttons_pressed.down) {
    	if(direction_currently_moving == up) {
    		final_direction_to_move = direction_currently_moving;
    	}
    	else final_direction_to_move = down;
    }
    else if(buttons_pressed.right) {
    	if(direction_currently_moving == left) {
    		final_direction_to_move = direction_currently_moving;
    	}
    	else final_direction_to_move = right;
    }
    else if(buttons_pressed.left) {
    	if(direction_currently_moving == right) {
    		final_direction_to_move = direction_currently_moving;
    	}
    	else final_direction_to_move = left;
    }
    else final_direction_to_move = direction_currently_moving; // No buttons have been pressed

    direction_currently_moving = final_direction_to_move;
    return final_direction_to_move;
}

void update_screen(snake_piece* head_of_snake, food_piece* food) {
    snake_piece* current_chunk = head_of_snake;

    //First put all of the snake pixels on the screen
    while(current_chunk != NULL) {
        LEDPANEL_writepixel(current_chunk->x_cord,
                            current_chunk->y_cord,
                            current_chunk->color);
        current_chunk = current_chunk->next;
    }
    if(food != NULL) {
        LEDPANEL_writepixel(food->x_cord,
                            food->y_cord,
                            GREEN);
    }
    LEDPANEL_updatepanel();
}

// Return true if one gametick has passed
bool check_gametick(u32 current_msecs, u32 last_msecs_updated) {
    const u32 msecs_between_update = 500;
    bool retval = false; 

    if( (current_msecs - last_msecs_updated) > msecs_between_update ) retval = true;

    return retval;
}
