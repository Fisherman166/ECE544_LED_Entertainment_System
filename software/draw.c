//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: draw.c
//
//*****************************************************************************
#include "draw.h"


//Globals
color draw_array [16][32];      //this represents the whole dang screen
u32 pencil_x = 15;              //pencil's x location in row
u32 pencil_y = 8;               //pencil's y location in column
buttons draw_buttons;           //button struct 
color pencil_color = white;     //current pencil color

/**
* run_draw
* runs the draw game
*/
void run_draw()
{
    u32 i, j;                   //counter indexes
    bool pencil_moved = false;  //true if pencil has moved this run through the loop
    
    //initialize array
    for(i=0; i <= DRAW_MAX_Y; i++)
    {
        for(j=0; j <= DRAW_MAX_X; j++)
        	draw_array[i][j] = off;
    }
    
    //draw
    while(true)
    {
        //read controller
        draw_buttons = read_controller(CONTROLLER1_DEV_ID);
        if(draw_buttons.start || draw_buttons.select)
            break;
    
        //move pencil
        pencil_moved = do_move_pencil();
        
        //cycle color 
        do_color_pencil(pencil_moved);
        
        //write to led panel
        do_write_panel();
    }
    return;
}


/**
* do_move_pencil
* Changes the location that the user is currently coloring
* @param - none
* @return - boolean whether or not a change has occurred
*/
bool do_move_pencil()
{
    bool change = false;
    
    if(draw_buttons.up && (pencil_y > DRAW_MIN_Y))
    {
        pencil_y = pencil_y + 1;
        change = true;
    }
    
    if(draw_buttons.down && (pencil_y < DRAW_MAX_Y))
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
* @param - bool new_loc: indicates if the location has changed
* @return - none
*/
void do_color_pencil(bool new_loc)
{
    if(new_loc)
    {
        pencil_color = draw_array[pencil_y][pencil_x];
        if(pencil_color == off)
            pencil_color = white;
    }
    
    if(draw_buttons.A)
    {
        if(pencil_color < white)
            pencil_color = pencil_color + 1;
        else
            pencil_color = off;
    }
    
    if(draw_buttons.B)
    {
            if(pencil_color > off)
                pencil_color = pencil_color - 1;
            else
                pencil_color = white;
    }
    
    draw_array[pencil_y][pencil_x] = pencil_color;
    
    return;
}

/**
* do_write_panel
* writes the entire array and the current pencil location to the panel
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
            LEDPANEL_writepixel(x, y, draw_array[y][x]);
        }
    }
    
    //blink pencil cursor (we might hate this)
    if(blink)
        LEDPANEL_writepixel(pencil_x, pencil_y, (u8) off);
    blink = !blink;
    
    //update led
    LEDPANEL_updatepanel();
    
    return;
}

