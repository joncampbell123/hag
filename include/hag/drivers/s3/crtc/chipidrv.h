//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ChipIDRevision_t;

namespace ChipIDRevision
{
    
    enum //Mask
    {
        RevisionStatus = 0x0F,
        ChipID = 0xF0               //When the software detects 0xE in ChipID field, it should then use CR2D, CR2E and
                                    //CR2F for chip ID information.
    };

    namespace Shift
    {
        enum
        {
            RevisionStatus = 0x00,
            ChipID = 0x04
        };
    }

    inline ChipIDRevision_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ChipIDRevision);
        return ChipIDRevision_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}}}
