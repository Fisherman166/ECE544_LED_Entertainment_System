//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: draw.c
//
//*****************************************************************************
#include "draw.h"


//*****************************************************************************
// Globals
//*****************************************************************************
color draw_array [16][32];      //this represents the whole dang screen
u32 pencil_x = 15;              //pencil's x location in row
u32 pencil_y = 8;               //pencil's y location in column
buttons draw_buttons;           //button struct 
color pencil_color = white;     //current pencil color


//*****************************************************************************
// Private function prototypes
//*****************************************************************************
bool do_move_pencil();
void do_color_pencil(bool new_loc);
void do_write_panel();
static bool check_gametick(u32 current_msecs, u32 last_msecs_updated);


//*****************************************************************************
// Functions
//*****************************************************************************
/**
* run_draw
* runs the draw game
* public
*/
void run_draw(u32* timestamp_msecs)
{
    u32 i, j;                           //counter indexes
    bool pencil_moved = false;          //true if pencil has moved this run through the loop
    u32 screen_updated_at_msecs = 0;    
    
    //initialize array
    for(i=0; i <= DRAW_MAX_Y; i++)
    {
        for(j=0; j <= DRAW_MAX_X; j++)
        	draw_array[i][j] = off;
    }
    //reset starting coordinates every time you restart drawing
    pencil_x = 15;
    pencil_y = 8;

    //draw
    while(true)
    {   
        // Handle the timestamp rollover case
    	if( *timestamp_msecs < screen_updated_at_msecs )  screen_updated_at_msecs = *timestamp_msecs;

        // Update the state of the game on a gametick
        if( check_gametick(*timestamp_msecs, screen_updated_at_msecs) ) 
        {                
            //read controller
            draw_buttons = read_controller(CONTROLLER1_DEV_ID);
            if(draw_buttons.select)
                break;
        
            //move pencil
            pencil_moved = do_move_pencil();
            
            //cycle color 
            do_color_pencil(pencil_moved);
            
            //write to led panel
            do_write_panel();
        }
    }
    return;
}


/**
* do_move_pencil
* Changes the location that the user is currently coloring
* private
* @param - none
* @return - boolean whether or not a change has occurred
*/
bool do_move_pencil()
{
    bool change = false;
    
    if(draw_buttons.up && (pencil_y < DRAW_MAX_Y))
    {
        pencil_y = pencil_y + 1;
        change = true;
    }
    
    if(draw_buttons.down && (pencil_y > DRAW_MIN_Y))
    {
        pencil_y = pencil_y - 1;
        change = true;
    }
    
    if(draw_buttons.right && (pencil_x < DRAW_MAX_X))
    {
        pencil_x = pencil_x + 1;
        change = true;
    }
    
    if(draw_buttons.left && (pencil_x > DRAW_MIN_X))
    {
        pencil_x = pencil_x - 1;
        change = true;
    }
        
    return change;
}

/**
* do_color_pencil
* Changes the color that the user is currently drawing
* private
* @param - bool new_loc: indicates if the location has changed
* @return - none
*/
void do_color_pencil(bool new_loc)
{   
    if(draw_buttons.A)
    {
        if(pencil_color < move)
            pencil_color = pencil_color + 1;
        else
            pencil_color = off;
    }
    
    if(draw_buttons.B)
    {
            if(pencil_color > off)
                pencil_color = pencil_color - 1;
            else
                pencil_color = move;
    }
    
    if(pencil_color != move)
        draw_array[pencil_y][pencil_x] = pencil_color;
    
    return;
}

/**
* do_write_panel
* writes the entire array and the current pencil location to the panel
* private
* @param - none
* @return - none
*/
void do_write_panel()
{
    static bool blink = 0;
    u8 x, y;
    
    //write entire array
    for(y = DRAW_MIN_Y; y <= DRAW_MAX_Y; y++)
    {
        for(x = DRAW_MIN_X; x <= DRAW_MAX_X; x++)
        {
        	if(draw_array[y][x] != off) LEDPANEL_writepixel(x, y, draw_array[y][x]);
        }
    }
    
    //blink pencil cursor to show where it is when not writing
    if((pencil_color == move) && blink)
        LEDPANEL_writepixel(pencil_x, pencil_y, (u8) white);
    blink = !blink;
    
    //update led
    LEDPANEL_updatepanel();
    
    return;
}

/**
* check_gametick
* Returns true if one gametick has passed
* private
* @param    - u32 current_msecs
*           - u32 last_msecs_updated
* @return bool enough time has passed
*/
static bool check_gametick(u32 current_msecs, u32 last_msecs_updated)
{
    const u32 msecs_between_update = 500;
    bool retval = false; 

    if( (current_msecs - last_msecs_updated) > msecs_between_update ) retval = true;

    return retval;
}

