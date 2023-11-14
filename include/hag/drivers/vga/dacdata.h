//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>


namespace Hag { namespace VGA
{

//This register is a data port to read or write the contents of the location in
//the color look-up table pointed to by the DAC Read Index or the
//DAC Write Index registers.

typedef uint8_t RAMDACData_t;

namespace RAMDACData
{
    
    enum //Mask
    {
        Address = 0xFF  //To prevent "snow flicker" on the screen, an application
                        //reading data from or writing data to the DAC Data
                        //register should ensure that the BLANK input to the RAMDAC
                        //is asserted. This can be accomplished either by restricting
                        //data transfers to retrace inter- vals, checking bit 3 of the
                        //Input Status 1 register (3?AH) to determine when retrace is
                        //occurring, or by using the screen-off bit in the Clocking
                        //Mode register of the se- quencer (bit 5 of SR1).
    };

    namespace Shift
    {
        enum
        {
            Address = 0x00
        };
    }

    inline RAMDACData_t Read()
    {
        return RAMDACData_t(SYS_ReadPortByte(Register::RAMDACData));
    }
    
    inline void Write(RAMDACData_t value)
    {
        SYS_WritePortByte(Register::RAMDACData, value);
    }
}

}}
