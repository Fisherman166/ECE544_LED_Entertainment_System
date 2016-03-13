//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: menu.c
//
//*****************************************************************************

#include "menu.h"


typedef struct  {
    u8 y_cord;
    u8 x_cord;
    u8 color;
} menu_image;


//*****************
// Functions
//*****************
static void draw_menu(menu_image*);
static bool check_menutick(u32 , u32 );



//*****************
// Menu Arrays
//*****************
menu_image  choose_image [] = {
{ 0 , 3 , 7 },
{ 0 , 4 , 7 },
{ 0 , 5 , 7 },
{ 0 , 7 , 7 },
{ 0 , 10 , 7 },
{ 0 , 12 , 7 },
{ 0 , 13 , 7 },
{ 0 , 14 , 7 },
{ 0 , 15 , 7 },
{ 0 , 17 , 7 },
{ 0 , 18 , 7 },
{ 0 , 19 , 7 },
{ 0 , 20 , 7 },
{ 0 , 22 , 7 },
{ 0 , 23 , 7 },
{ 0 , 24 , 7 },
{ 0 , 25 , 7 },
{ 0 , 27 , 7 },
{ 0 , 28 , 7 },
{ 0 , 29 , 7 },
{ 1 , 2 , 7 },
{ 1 , 7 , 7 },
{ 1 , 10 , 7 },
{ 1 , 12 , 7 },
{ 1 , 15 , 7 },
{ 1 , 17 , 7 },
{ 1 , 20 , 7 },
{ 1 , 22 , 7 },
{ 1 , 27 , 7 },
{ 2 , 2 , 7 },
{ 2 , 7 , 7 },
{ 2 , 8 , 7 },
{ 2 , 9 , 7 },
{ 2 , 10 , 7 },
{ 2 , 12 , 7 },
{ 2 , 15 , 7 },
{ 2 , 17 , 7 },
{ 2 , 20 , 7 },
{ 2 , 23 , 7 },
{ 2 , 24 , 7 },
{ 2 , 27 , 7 },
{ 2 , 28 , 7 },
{ 2 , 29 , 7 },
{ 3 , 2 , 7 },
{ 3 , 7 , 7 },
{ 3 , 10 , 7 },
{ 3 , 12 , 7 },
{ 3 , 15 , 7 },
{ 3 , 17 , 7 },
{ 3 , 20 , 7 },
{ 3 , 25 , 7 },
{ 3 , 27 , 7 },
{ 4 , 3 , 7 },
{ 4 , 4 , 7 },
{ 4 , 5 , 7 },
{ 4 , 7 , 7 },
{ 4 , 10 , 7 },
{ 4 , 12 , 7 },
{ 4 , 13 , 7 },
{ 4 , 14 , 7 },
{ 4 , 15 , 7 },
{ 4 , 17 , 7 },
{ 4 , 18 , 7 },
{ 4 , 19 , 7 },
{ 4 , 20 , 7 },
{ 4 , 22 , 7 },
{ 4 , 23 , 7 },
{ 4 , 24 , 7 },
{ 4 , 25 , 7 },
{ 4 , 27 , 7 },
{ 4 , 28 , 7 },
{ 4 , 29 , 7 },
{ 6 , 7 , 7 },
{ 6 , 8 , 7 },
{ 6 , 9 , 7 },
{ 6 , 12 , 7 },
{ 6 , 13 , 7 },
{ 6 , 17 , 7 },
{ 6 , 19 , 7 },
{ 6 , 22 , 7 },
{ 6 , 23 , 7 },
{ 6 , 24 , 7 },
{ 7 , 6 , 7 },
{ 7 , 11 , 7 },
{ 7 , 14 , 7 },
{ 7 , 16 , 7 },
{ 7 , 18 , 7 },
{ 7 , 20 , 7 },
{ 7 , 22 , 7 },
{ 8 , 6 , 7 },
{ 8 , 8 , 7 },
{ 8 , 9 , 7 },
{ 8 , 11 , 7 },
{ 8 , 12 , 7 },
{ 8 , 13 , 7 },
{ 8 , 14 , 7 },
{ 8 , 16 , 7 },
{ 8 , 18 , 7 },
{ 8 , 20 , 7 },
{ 8 , 22 , 7 },
{ 8 , 23 , 7 },
{ 8 , 24 , 7 },
{ 9 , 6 , 7 },
{ 9 , 9 , 7 },
{ 9 , 11 , 7 },
{ 9 , 14 , 7 },
{ 9 , 16 , 7 },
{ 9 , 18 , 7 },
{ 9 , 20 , 7 },
{ 9 , 22 , 7 },
{ 10 , 7 , 7 },
{ 10 , 8 , 7 },
{ 10 , 9 , 7 },
{ 10 , 11 , 7 },
{ 10 , 14 , 7 },
{ 10 , 16 , 7 },
{ 10 , 18 , 7 },
{ 10 , 20 , 7 },
{ 10 , 22 , 7 },
{ 10 , 23 , 7 },
{ 10 , 24 , 7 },
{ 11 , 2 , 7 },
{ 11 , 29 , 7 },
{ 12 , 1 , 7 },
{ 12 , 2 , 7 },
{ 12 , 29 , 7 },
{ 12 , 30 , 7 },
{ 13 , 0 , 7 },
{ 13 , 1 , 7 },
{ 13 , 2 , 7 },
{ 13 , 3 , 7 },
{ 13 , 4 , 7 },
{ 13 , 5 , 7 },
{ 13 , 26 , 7 },
{ 13 , 27 , 7 },
{ 13 , 28 , 7 },
{ 13 , 29 , 7 },
{ 13 , 30 , 7 },
{ 13 , 31 , 7 },
{ 14 , 1 , 7 },
{ 14 , 2 , 7 },
{ 14 , 29 , 7 },
{ 14 , 30 , 7 },
{ 15 , 2 , 7 },
{ 15 , 29 , 7 },
{ 0 , 0, 8 } 
};

menu_image  snake_image [] = {
{ 1 , 1 , 2 },
{ 1 , 2 , 2 },
{ 1 , 3 , 2 },
{ 1 , 4 , 2 },
{ 1 , 5 , 2 },
{ 1 , 6 , 2 },
{ 1 , 7 , 2 },
{ 1 , 21 , 2 },
{ 1 , 25 , 2 },
{ 2 , 1 , 2 },
{ 2 , 21 , 2 },
{ 2 , 24 , 2 },
{ 3 , 1 , 2 },
{ 3 , 9 , 2 },
{ 3 , 10 , 2 },
{ 3 , 11 , 2 },
{ 3 , 12 , 2 },
{ 3 , 13 , 2 },
{ 3 , 15 , 2 },
{ 3 , 16 , 2 },
{ 3 , 17 , 2 },
{ 3 , 18 , 2 },
{ 3 , 19 , 2 },
{ 3 , 21 , 2 },
{ 3 , 23 , 2 },
{ 3 , 27 , 2 },
{ 3 , 28 , 2 },
{ 3 , 29 , 2 },
{ 3 , 30 , 2 },
{ 4 , 1 , 2 },
{ 4 , 2 , 2 },
{ 4 , 3 , 2 },
{ 4 , 4 , 2 },
{ 4 , 5 , 2 },
{ 4 , 6 , 2 },
{ 4 , 7 , 2 },
{ 4 , 9 , 2 },
{ 4 , 13 , 2 },
{ 4 , 15 , 2 },
{ 4 , 19 , 2 },
{ 4 , 21 , 2 },
{ 4 , 22 , 2 },
{ 4 , 27 , 2 },
{ 4 , 30 , 2 },
{ 5 , 7 , 2 },
{ 5 , 9 , 2 },
{ 5 , 13 , 2 },
{ 5 , 15 , 2 },
{ 5 , 19 , 2 },
{ 5 , 21 , 2 },
{ 5 , 23 , 2 },
{ 5 , 27 , 2 },
{ 5 , 28 , 2 },
{ 5 , 29 , 2 },
{ 5 , 30 , 2 },
{ 6 , 7 , 2 },
{ 6 , 9 , 2 },
{ 6 , 13 , 2 },
{ 6 , 15 , 2 },
{ 6 , 19 , 2 },
{ 6 , 21 , 2 },
{ 6 , 24 , 2 },
{ 6 , 27 , 2 },
{ 7 , 7 , 2 },
{ 7 , 9 , 2 },
{ 7 , 13 , 2 },
{ 7 , 15 , 2 },
{ 7 , 16 , 2 },
{ 7 , 17 , 2 },
{ 7 , 18 , 2 },
{ 7 , 19 , 2 },
{ 7 , 21 , 2 },
{ 7 , 25 , 2 },
{ 7 , 27 , 2 },
{ 8 , 1 , 2 },
{ 8 , 2 , 2 },
{ 8 , 3 , 2 },
{ 8 , 4 , 2 },
{ 8 , 5 , 2 },
{ 8 , 6 , 2 },
{ 8 , 7 , 2 },
{ 8 , 9 , 2 },
{ 8 , 13 , 2 },
{ 8 , 14 , 2 },
{ 8 , 15 , 2 },
{ 8 , 16 , 2 },
{ 8 , 17 , 2 },
{ 8 , 18 , 2 },
{ 8 , 19 , 2 },
{ 8 , 20 , 2 },
{ 8 , 21 , 2 },
{ 8 , 26 , 2 },
{ 8 , 27 , 2 },
{ 8 , 28 , 2 },
{ 8 , 29 , 2 },
{ 8 , 30 , 2 },
{ 9 , 1 , 2 },
{ 9 , 9 , 2 },
{ 9 , 30 , 2 },
{ 10 , 1 , 2 },
{ 10 , 2 , 2 },
{ 10 , 3 , 2 },
{ 10 , 4 , 2 },
{ 10 , 5 , 2 },
{ 10 , 6 , 2 },
{ 10 , 7 , 2 },
{ 10 , 8 , 2 },
{ 10 , 9 , 2 },
{ 10 , 11 , 4 },
{ 10 , 13 , 1 },
{ 10 , 14 , 2 },
{ 10 , 15 , 2 },
{ 10 , 16 , 2 },
{ 10 , 17 , 2 },
{ 10 , 18 , 2 },
{ 10 , 19 , 2 },
{ 10 , 20 , 2 },
{ 10 , 21 , 2 },
{ 10 , 22 , 2 },
{ 10 , 23 , 2 },
{ 10 , 24 , 2 },
{ 10 , 25 , 2 },
{ 10 , 26 , 2 },
{ 10 , 27 , 2 },
{ 10 , 28 , 2 },
{ 10 , 29 , 2 },
{ 10 , 30 , 2 },
{ 11 , 2 , 7 },
{ 11 , 29 , 7 },
{ 12 , 1 , 7 },
{ 12 , 2 , 7 },
{ 12 , 29 , 7 },
{ 12 , 30 , 7 },
{ 13 , 0 , 7 },
{ 13 , 1 , 7 },
{ 13 , 2 , 7 },
{ 13 , 3 , 7 },
{ 13 , 4 , 7 },
{ 13 , 5 , 7 },
{ 13 , 26 , 7 },
{ 13 , 27 , 7 },
{ 13 , 28 , 7 },
{ 13 , 29 , 7 },
{ 13 , 30 , 7 },
{ 13 , 31 , 7 },
{ 14 , 1 , 7 },
{ 14 , 2 , 7 },
{ 14 , 29 , 7 },
{ 14 , 30 , 7 },
{ 15 , 2 , 7 },
{ 15 , 29 , 7 },
{ 0 , 0, 8 } 
};

menu_image  pong_image [] = {
{ 0 , 0 , 4 },
{ 1 , 0 , 4 },
{ 2 , 0 , 4 },
{ 2 , 2 , 7 },
{ 3 , 0 , 4 },
{ 4 , 0 , 4 },
{ 5 , 6 , 7 },
{ 5 , 7 , 7 },
{ 5 , 8 , 7 },
{ 5 , 9 , 7 },
{ 5 , 11 , 7 },
{ 5 , 12 , 7 },
{ 5 , 13 , 7 },
{ 5 , 14 , 7 },
{ 5 , 16 , 7 },
{ 5 , 20 , 7 },
{ 5 , 23 , 7 },
{ 5 , 24 , 7 },
{ 5 , 31 , 1 },
{ 6 , 6 , 7 },
{ 6 , 9 , 7 },
{ 6 , 11 , 7 },
{ 6 , 14 , 7 },
{ 6 , 16 , 7 },
{ 6 , 17 , 7 },
{ 6 , 20 , 7 },
{ 6 , 22 , 7 },
{ 6 , 25 , 7 },
{ 6 , 31 , 1 },
{ 7 , 6 , 7 },
{ 7 , 9 , 7 },
{ 7 , 11 , 7 },
{ 7 , 14 , 7 },
{ 7 , 16 , 7 },
{ 7 , 18 , 7 },
{ 7 , 20 , 7 },
{ 7 , 22 , 7 },
{ 7 , 31 , 1 },
{ 8 , 6 , 7 },
{ 8 , 7 , 7 },
{ 8 , 8 , 7 },
{ 8 , 9 , 7 },
{ 8 , 11 , 7 },
{ 8 , 14 , 7 },
{ 8 , 16 , 7 },
{ 8 , 19 , 7 },
{ 8 , 20 , 7 },
{ 8 , 22 , 7 },
{ 8 , 24 , 7 },
{ 8 , 25 , 7 },
{ 8 , 31 , 1 },
{ 9 , 6 , 7 },
{ 9 , 11 , 7 },
{ 9 , 14 , 7 },
{ 9 , 16 , 7 },
{ 9 , 20 , 7 },
{ 9 , 22 , 7 },
{ 9 , 25 , 7 },
{ 9 , 31 , 1 },
{ 10 , 6 , 7 },
{ 10 , 11 , 7 },
{ 10 , 14 , 7 },
{ 10 , 16 , 7 },
{ 10 , 20 , 7 },
{ 10 , 22 , 7 },
{ 10 , 25 , 7 },
{ 11 , 2 , 7 },
{ 11 , 6 , 7 },
{ 11 , 11 , 7 },
{ 11 , 12 , 7 },
{ 11 , 13 , 7 },
{ 11 , 14 , 7 },
{ 11 , 16 , 7 },
{ 11 , 20 , 7 },
{ 11 , 23 , 7 },
{ 11 , 24 , 7 },
{ 11 , 29 , 7 },
{ 12 , 1 , 7 },
{ 12 , 2 , 7 },
{ 12 , 29 , 7 },
{ 12 , 30 , 7 },
{ 13 , 0 , 7 },
{ 13 , 1 , 7 },
{ 13 , 2 , 7 },
{ 13 , 3 , 7 },
{ 13 , 4 , 7 },
{ 13 , 5 , 7 },
{ 13 , 26 , 7 },
{ 13 , 27 , 7 },
{ 13 , 28 , 7 },
{ 13 , 29 , 7 },
{ 13 , 30 , 7 },
{ 13 , 31 , 7 },
{ 14 , 1 , 7 },
{ 14 , 2 , 7 },
{ 14 , 29 , 7 },
{ 14 , 30 , 7 },
{ 15 , 2 , 7 },
{ 15 , 29 , 7 },
{ 0 , 0, 8 } 
};

menu_image  draw_image [] = {
{ 0 , 1 , 3 },
{ 0 , 2 , 6 },
{ 0 , 3 , 3 },
{ 0 , 4 , 6 },
{ 0 , 5 , 3 },
{ 0 , 6 , 6 },
{ 0 , 7 , 3 },
{ 0 , 9 , 3 },
{ 0 , 11 , 3 },
{ 0 , 13 , 3 },
{ 0 , 15 , 3 },
{ 0 , 17 , 3 },
{ 0 , 19 , 3 },
{ 0 , 21 , 3 },
{ 0 , 23 , 3 },
{ 0 , 25 , 3 },
{ 0 , 27 , 3 },
{ 0 , 29 , 3 },
{ 0 , 31 , 3 },
{ 1 , 0 , 3 },
{ 1 , 2 , 3 },
{ 1 , 3 , 6 },
{ 1 , 4 , 6 },
{ 1 , 5 , 6 },
{ 1 , 6 , 3 },
{ 1 , 8 , 3 },
{ 1 , 10 , 3 },
{ 1 , 12 , 3 },
{ 1 , 14 , 3 },
{ 1 , 16 , 3 },
{ 1 , 18 , 3 },
{ 1 , 20 , 3 },
{ 1 , 22 , 3 },
{ 1 , 24 , 3 },
{ 1 , 26 , 3 },
{ 1 , 28 , 3 },
{ 1 , 30 , 3 },
{ 2 , 1 , 3 },
{ 2 , 2 , 6 },
{ 2 , 3 , 6 },
{ 2 , 4 , 6 },
{ 2 , 5 , 6 },
{ 2 , 6 , 6 },
{ 2 , 7 , 3 },
{ 2 , 9 , 3 },
{ 2 , 11 , 2 },
{ 2 , 13 , 3 },
{ 2 , 15 , 3 },
{ 2 , 17 , 3 },
{ 2 , 19 , 3 },
{ 2 , 21 , 3 },
{ 2 , 23 , 2 },
{ 2 , 25 , 3 },
{ 2 , 27 , 3 },
{ 2 , 29 , 3 },
{ 2 , 31 , 3 },
{ 3 , 0 , 3 },
{ 3 , 2 , 3 },
{ 3 , 3 , 6 },
{ 3 , 4 , 6 },
{ 3 , 5 , 6 },
{ 3 , 6 , 3 },
{ 3 , 8 , 3 },
{ 3 , 10 , 2 },
{ 3 , 11 , 2 },
{ 3 , 12 , 2 },
{ 3 , 14 , 3 },
{ 3 , 16 , 3 },
{ 3 , 18 , 3 },
{ 3 , 20 , 3 },
{ 3 , 22 , 2 },
{ 3 , 23 , 2 },
{ 3 , 24 , 2 },
{ 3 , 26 , 3 },
{ 3 , 28 , 3 },
{ 3 , 30 , 3 },
{ 4 , 1 , 3 },
{ 4 , 2 , 6 },
{ 4 , 3 , 3 },
{ 4 , 4 , 6 },
{ 4 , 5 , 3 },
{ 4 , 6 , 6 },
{ 4 , 7 , 3 },
{ 4 , 9 , 3 },
{ 4 , 10 , 2 },
{ 4 , 11 , 2 },
{ 4 , 12 , 2 },
{ 4 , 13 , 3 },
{ 4 , 15 , 3 },
{ 4 , 17 , 3 },
{ 4 , 19 , 3 },
{ 4 , 21 , 3 },
{ 4 , 22 , 2 },
{ 4 , 23 , 2 },
{ 4 , 24 , 2 },
{ 4 , 25 , 3 },
{ 4 , 27 , 3 },
{ 4 , 29 , 3 },
{ 4 , 31 , 3 },
{ 5 , 0 , 3 },
{ 5 , 2 , 3 },
{ 5 , 4 , 3 },
{ 5 , 6 , 3 },
{ 5 , 8 , 3 },
{ 5 , 9 , 2 },
{ 5 , 10 , 2 },
{ 5 , 11 , 2 },
{ 5 , 12 , 2 },
{ 5 , 13 , 2 },
{ 5 , 14 , 3 },
{ 5 , 16 , 3 },
{ 5 , 18 , 3 },
{ 5 , 20 , 3 },
{ 5 , 21 , 2 },
{ 5 , 22 , 2 },
{ 5 , 23 , 2 },
{ 5 , 24 , 2 },
{ 5 , 25 , 2 },
{ 5 , 26 , 3 },
{ 5 , 28 , 3 },
{ 5 , 30 , 3 },
{ 6 , 1 , 3 },
{ 6 , 3 , 3 },
{ 6 , 5 , 3 },
{ 6 , 7 , 3 },
{ 6 , 9 , 3 },
{ 6 , 11 , 2 },
{ 6 , 13 , 3 },
{ 6 , 15 , 3 },
{ 6 , 17 , 3 },
{ 6 , 19 , 3 },
{ 6 , 21 , 3 },
{ 6 , 23 , 2 },
{ 6 , 25 , 3 },
{ 6 , 27 , 3 },
{ 6 , 29 , 3 },
{ 6 , 31 , 3 },
{ 7 , 6 , 7 },
{ 7 , 7 , 7 },
{ 7 , 8 , 7 },
{ 7 , 11 , 7 },
{ 7 , 12 , 7 },
{ 7 , 13 , 7 },
{ 7 , 14 , 7 },
{ 7 , 17 , 7 },
{ 7 , 18 , 7 },
{ 7 , 21 , 7 },
{ 7 , 23 , 7 },
{ 7 , 25 , 7 },
{ 8 , 6 , 7 },
{ 8 , 9 , 7 },
{ 8 , 11 , 7 },
{ 8 , 14 , 7 },
{ 8 , 16 , 7 },
{ 8 , 19 , 7 },
{ 8 , 21 , 7 },
{ 8 , 23 , 7 },
{ 8 , 25 , 7 },
{ 9 , 6 , 7 },
{ 9 , 9 , 7 },
{ 9 , 11 , 7 },
{ 9 , 14 , 7 },
{ 9 , 16 , 7 },
{ 9 , 19 , 7 },
{ 9 , 21 , 7 },
{ 9 , 23 , 7 },
{ 9 , 25 , 7 },
{ 10 , 6 , 7 },
{ 10 , 9 , 7 },
{ 10 , 11 , 7 },
{ 10 , 12 , 7 },
{ 10 , 13 , 7 },
{ 10 , 14 , 7 },
{ 10 , 16 , 7 },
{ 10 , 17 , 7 },
{ 10 , 18 , 7 },
{ 10 , 19 , 7 },
{ 10 , 21 , 7 },
{ 10 , 23 , 7 },
{ 10 , 25 , 7 },
{ 11 , 2 , 7 },
{ 11 , 6 , 7 },
{ 11 , 9 , 7 },
{ 11 , 11 , 7 },
{ 11 , 13 , 7 },
{ 11 , 16 , 7 },
{ 11 , 19 , 7 },
{ 11 , 21 , 7 },
{ 11 , 23 , 7 },
{ 11 , 25 , 7 },
{ 11 , 29 , 7 },
{ 12 , 1 , 7 },
{ 12 , 2 , 7 },
{ 12 , 6 , 7 },
{ 12 , 7 , 7 },
{ 12 , 8 , 7 },
{ 12 , 11 , 7 },
{ 12 , 14 , 7 },
{ 12 , 16 , 7 },
{ 12 , 19 , 7 },
{ 12 , 22 , 7 },
{ 12 , 24 , 7 },
{ 12 , 29 , 7 },
{ 12 , 30 , 7 },
{ 13 , 0 , 7 },
{ 13 , 1 , 7 },
{ 13 , 2 , 7 },
{ 13 , 3 , 7 },
{ 13 , 4 , 7 },
{ 13 , 5 , 7 },
{ 13 , 26 , 7 },
{ 13 , 27 , 7 },
{ 13 , 28 , 7 },
{ 13 , 29 , 7 },
{ 13 , 30 , 7 },
{ 13 , 31 , 7 },
{ 14 , 1 , 7 },
{ 14 , 2 , 7 },
{ 14 , 29 , 7 },
{ 14 , 30 , 7 },
{ 15 , 2 , 7 },
{ 15 , 29 , 7 },
{ 0 , 0, 8 } 
};



//*****************************************************************************
// Reads controllers and updates menu screens on the LED panel
//*****************************************************************************
u8 select_game(u32* timestamp_msecs) {
  u8 current_menu = CHOOSE_NUM;
  buttons buttons_pressed;
  u32 screen_updated_at_msecs = 0;

  draw_menu(choose_image);

  for(;;) {
    // Handle the timestamp rollover case
    if( *timestamp_msecs < screen_updated_at_msecs )  screen_updated_at_msecs = *timestamp_msecs;

    // Update the state of the game on a gametick
    if( check_menutick(*timestamp_msecs, screen_updated_at_msecs) ) {
      screen_updated_at_msecs = *timestamp_msecs;

    buttons_pressed = read_controller(CONTROLLER1_DEV_ID);

    if(current_menu == CHOOSE_NUM) {
        if(buttons_pressed.right) {
          current_menu = SNAKE_NUM;
          draw_menu(snake_image);
        }
        else if(buttons_pressed.left) {
          current_menu = DRAW_NUM;
          draw_menu(draw_image);
        }
    }
    else if(current_menu == SNAKE_NUM) {
        if(buttons_pressed.start) return SNAKE_NUM;
        else if(buttons_pressed.right) {
          current_menu = PONG_NUM;
          draw_menu(pong_image);
        }
        else if(buttons_pressed.left) {
          current_menu = DRAW_NUM;
          draw_menu(draw_image);
        }
    }
    else if(current_menu == PONG_NUM) {
        if(buttons_pressed.start) return PONG_NUM;
        else if(buttons_pressed.right) {
          current_menu = DRAW_NUM;
          draw_menu(draw_image);
        }
        else if(buttons_pressed.left) {
          current_menu = SNAKE_NUM;
          draw_menu(snake_image);
        }
    }
    else if(current_menu == DRAW_NUM) {
        if(buttons_pressed.start) return DRAW_NUM;
        else if(buttons_pressed.right) {
          current_menu = SNAKE_NUM;
          draw_menu(snake_image);
        }
        else if(buttons_pressed.left) {
          current_menu = PONG_NUM;
          draw_menu(pong_image);
        }
    }
    }
  }


}


//*****************************************************************************
// Draws menu image on the LED panel
//*****************************************************************************
static void draw_menu(menu_image* current_menu) {

	int i = 0;

  while(current_menu[i].color != 8) {
    LEDPANEL_writepixel(current_menu[i].x_cord,
                        current_menu[i].y_cord,
                        current_menu[i].color);

    i++;
  }

  LEDPANEL_updatepanel();

}

//*****************************************************************************
// Returns true if one gametick has passed
//*****************************************************************************
static bool check_menutick(u32 current_msecs, u32 last_msecs_updated) {
    const u32 msecs_between_update = 500;
    bool retval = false;

    if( (current_msecs - last_msecs_updated) > msecs_between_update ) retval = true;

    return retval;
}
