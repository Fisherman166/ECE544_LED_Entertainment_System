

/***************************** Include Files *******************************/
#include "ledpanel.h"

/**************************** Global Variables *****************************/
u32 LEDPANEL_BaseAddress;       //base address for led panel

/************************** Function Definitions ***************************/
/**
* Initialize LED panel with base register address
* @param baseaddr_p - 32 bit pointer to base register address
* @return 
*    - XST_SUCCESS   if all self-test code passed
*    - XST_FAILURE   if any self-test code failed
*/
XST_SUCCESS LEDPANEL_initialize(void * baseaddr_p)
{
    LEDPANEL_BaseAddress = (u32) baseaddr_p;
    
    return (XStatus LEDPANEL_Reg_SelfTest(baseaddr_p));
}


/**
* Signals to the hw that there is new data to saved for led panel
* @param none
* @return XST_SUCCESS   
*/
XST_SUCCESS LEDPANEL_updatepanel()
{
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_update_panel_OFFSET, 0x00000001);
    sleep(1);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_update_panel_OFFSET, 0x00000000);
    return XST_SUCCESS;
}



/**
* Signals to the hw that there is new data to saved for led panel
* @param x, y, color
* @return XST_SUCCESS   
*/
XST_SUCCESS LEDPANEL_writepixel(u8 x, u8 y, u8 color)
{
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_color_OFFSET, (u32) color);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_x_address_OFFSET, (u32) x);
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_x_address_OFFSET, (u32) y);

    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_new_data_OFFSET, 0x00000001);
    sleep(1);    
    LEDPANEL_mWriteReg(LEDPANEL_BaseAddress, LEDPANEL_ledpanel_new_data_OFFSET, 0x00000000);

    return XST_SUCCESS;
}
