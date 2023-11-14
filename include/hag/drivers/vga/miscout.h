//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace VGA
{

typedef uint8_t MiscellaneousOutput_t;

namespace MiscellaneousOutput
{
    enum //Mask
    {
        IOAddressSelect = 0x1,                              //IOA SEL 0 = Monochrome, 1 = Color
        EnableCPUDisplayMemoryAccess = 0x02,                //ENB RAM 0 = Disable, 1 = Enable
        ClockSelect = 0x0C,                                 //00 = 25.175MHz DCLK for 640 horizontal pixels
                                                            //11 = 28.322MHz DCLK for 720 horizontal pixels
                                                            //10 = Reserved
                                                            //11 = Enables loading of DCLK PLL parameters in SR12 and SR13.
        PageSelect = 0x20,                                  //PGSL Select High 64K Page, 0 = low 64k page, 1 = high 64k page
        SelectNegativeHorizontalSyncPulse = 0x40,           //~HSP 0 = Select a positive horizontal retrace sync pulse, 1 = Select negative...
        SelectNegativeVerticalSyncPulse = 0x80              //~VSP 0 = Select a positive vertical retrace sync pulse, 1 = Select negative...
    };

    namespace Shift
    {
        enum
        {
            IOAddressSelect = 0x00,
            EnableCPUDisplayMemoryAccess = 0x01,
            ClockSelect = 0x02,
            PageSelect = 0x05,
            SelectNegativeHorizontalSyncPulse = 0x06,
            SelectNegativeVerticalSyncPulse = 0x07
        };
    }

    inline MiscellaneousOutput_t Read()
    {
        return MiscellaneousOutput_t(SYS_ReadPortByte(Register::MiscellaneousR));
    }
    
    inline void Write(MiscellaneousOutput_t value)
    {
        SYS_WritePortByte(Register::MiscellaneousW, value);
    }

}

}}
