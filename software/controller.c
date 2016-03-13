//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: controller.c
//
//*****************************************************************************

#include "controller.h"

buttons read_controller(u16 controller_device_id) {
    const u8 right_button_mask = 0x1;
    const u8 left_button_mask = 0x2;
    const u8 up_button_mask = 0x4;
    const u8 down_button_mask = 0x8;
    const u8 start_button_mask = 0x10;
    const u8 select_button_mask = 0x20;
    const u8 B_button_mask = 0x40;
    const u8 A_button_mask = 0x80;
    buttons controller = {false, false, false, false,
                          false, false, false, false};

    u8 controller_value = NES_read(controller_device_id);

    controller_value = ~controller_value;

    if(controller_value & right_button_mask) controller.right = true;
    if(controller_value & left_button_mask) controller.left = true;
    if(controller_value & up_button_mask) controller.up = true;
    if(controller_value & down_button_mask) controller.down = true;
    if(controller_value & start_button_mask) controller.start = true;
    if(controller_value & select_button_mask) controller.select = true;
    if(controller_value & B_button_mask) controller.B = true;
    if(controller_value & A_button_mask) controller.A = true;
    
    return controller;
}

