

/***************************** Include Files *******************************/
#include "ledpanel.h"

/**************************** Global Variables *****************************/
u32 LEDPANEL_BaseAddress;       //base address for led panel

/************************** Function Definitions ***************************/
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
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_update_panel_OFFSET, 0x00000001);
    xil_printf("UPDATING PANEL\n");
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
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_color_OFFSET, (u32) color);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_x_address_OFFSET, (u32) x);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_y_address_OFFSET, (u32) y);

    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_new_data_OFFSET, 0x00000001);
    xil_printf("WRITING PIXEL\n");
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_new_data_OFFSET, 0x00000000);

    return XST_SUCCESS;
}
