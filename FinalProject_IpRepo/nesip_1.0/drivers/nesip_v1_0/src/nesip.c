

/***************************** Include Files *******************************/
#include "nesip.h"

/************************** Function Definitions ***************************/
#define MAX_NUM_CONTROLLERS 4

// Highly unlikely that the baseaddress for the device will ever be at
// address 0
int BaseAddress[MAX_NUM_CONTROLLERS] = {0,0,0,0};

int NES_initialize(u8 devid, u32 baseaddress)
{
    if(devid >= MAX_NUM_CONTROLLERS) return XST_FAILURE;
    BaseAddress[devid] = baseaddress;
    return XST_SUCCESS;    
}

// The NES controller hardware is active low.
// We want active high signals
u8 NES_read(u8 devid)
{
    if(devid >= MAX_NUM_CONTROLLERS) return XST_FAILURE;
    if(BaseAddress[devid] == 0) return XST_FAILURE;
    u8 nes_value = (u8) NESIP_mReadReg(BaseAddress[devid], NESIP_NES_SLV_REG0_OFFSET);

    return ~nes_value;
}
