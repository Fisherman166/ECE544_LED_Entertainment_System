

/***************************** Include Files *******************************/
#include "nesip.h"

/************************** Function Definitions ***************************/
int BaseAddress[4];


int NES_initialize( u8 devid, u32 baseaddress)
{
    if(devid >= 4)
        return XST_FAILURE;
    BaseAddress[devid] = baseaddress;
    return XST_SUCCESS;    
}


u8 NES_read(u8 devid)
{
    
    if(devid >= 4)
        return XST_FAILURE;
    return (u8) NESIP_mReadReg(BaseAddress[devid], NESIP_NES_SLV_REG0_OFFSET);
    
}