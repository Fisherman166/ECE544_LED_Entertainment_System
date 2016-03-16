//*****************************************************************************
//
// Project: ECE544 Final Project
// Team: Sean Koppenhafer, Nathan Morelli, Jessica Bare
// File: nesip.c
//
// Driver functions for the NES controller IP.  
//
//*****************************************************************************

#include "nesip.h"

//*****************************************************************************
// Global Base Address for up to 4 controllers
//*****************************************************************************
int BaseAddress[4];


//*****************************************************************************
// Functions
//*****************************************************************************

//*****************************************************************************
// Initialize function assigns base address to the element in the BaseAddress
// array indexed by the current controllers device ID.
//*****************************************************************************
int NES_initialize( u8 devid, u32 baseaddress)
{
    if(devid >= 4)
        return XST_FAILURE;
    BaseAddress[devid] = baseaddress;
    return XST_SUCCESS;    
}


//*****************************************************************************
// Calls the mReadReg macro using the device ID to index into the BaseAddress Array
//*****************************************************************************
u8 NES_read(u8 devid)
{
    
    if(devid >= 4)
        return XST_FAILURE;
    return (u8) NESIP_mReadReg(BaseAddress[devid], NESIP_NES_SLV_REG0_OFFSET);
    
}