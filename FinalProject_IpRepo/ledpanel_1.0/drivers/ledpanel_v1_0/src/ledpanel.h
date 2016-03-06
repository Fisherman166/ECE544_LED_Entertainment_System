
#ifndef LEDPANEL_H
#define LEDPANEL_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"

#define LEDPANEL_ledpanel_x_address_OFFSET 0
#define LEDPANEL_ledpanel_y_address_OFFSET 4
#define LEDPANEL_ledpanel_color_OFFSET 8
#define LEDPANEL_ledpanel_update_panel_OFFSET 12
#define LEDPANEL_ledpanel_new_data_OFFSET 16


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a LEDPANEL register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the LEDPANELdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void LEDPANEL_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define LEDPANEL_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a LEDPANEL register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the LEDPANEL device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 LEDPANEL_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define LEDPANEL_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the LEDPANEL instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus LEDPANEL_Reg_SelfTest(void * baseaddr_p);

/**
* Initialize LED panel with base register address
* @param baseaddr_p - 32 bit pointer to base register address
* @return 
*    - XST_SUCCESS   if all self-test code passed
*    - XST_FAILURE   if any self-test code failed
*/
XST_SUCCESS LEDPANEL_initialize(void * baseaddr_p);

/**
* Signals to the hw that there is new data to saved for led panel
* @param x, y, color
* @return XST_SUCCESS   
*/
XST_SUCCESS LEDPANEL_writepixel(u8 x, u8 y, u8 color);

/**
* Signals to the hw that there is new data to saved for led panel
* @param none
* @return XST_SUCCESS   
*/
XST_SUCCESS LEDPANEL_updatepanel();

#endif // LEDPANEL_H
