//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: ledpanel.c
//
// Driver functions for the LED Panel IP.  
//
//*****************************************************************************

#include "ledpanel.h"

//*****************************************************************************
// Global Base Address for LED Panel
//*****************************************************************************
u32 LEDPANEL_BaseAddress;


//*****************************************************************************
// Functions
//*****************************************************************************

/**
* Initialize LED panel with base register address
* @param baseaddr - base register address
* @return 
*    - XST_SUCCESS   if all self-test code passed
*    - XST_FAILURE   if any self-test code failed
*/
XStatus LEDPANEL_initialize(u32 baseaddr)
{
    LEDPANEL_BaseAddress = baseaddr;
    
    return LEDPANEL_Reg_SelfTest(LEDPANEL_BaseAddress);
}


/**
* Signals to the hw that there is new data to saved for led panel
* @param none
* @return XST_SUCCESS   
*/
XStatus LEDPANEL_updatepanel()
{
    //Assert the updatepanel flag and then reset it to 0
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_update_panel_OFFSET, 0x00000001);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_update_panel_OFFSET, 0x00000000);
    
    return XST_SUCCESS;
}



/**
* Signals to the hw that there is new data to saved for led panel
* @param x, y, color
* @return XST_SUCCESS   
*/
XStatus LEDPANEL_writepixel(u8 x, u8 y, u8 color)
{
    //Call mWriteReg macro to write the x_address, y_address and color values
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_color_OFFSET, (u32) color);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_x_address_OFFSET, (u32) x);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_y_address_OFFSET, (u32) y);

    //Assert the new_data flag and then reset it to 0
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_new_data_OFFSET, 0x00000001);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_new_data_OFFSET, 0x00000000);

    return XST_SUCCESS;
}
