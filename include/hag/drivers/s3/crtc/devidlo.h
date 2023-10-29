//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

//This register allows software to identify whether a Trio32 or Trio64 is present.
//Trio32 0x10, Trio64 0x11

typedef uint8_t DeviceIDLow_t;

namespace DeviceIDLow
{

    inline DeviceIDLow_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::DeviceIDLow);
        return DeviceIDLow_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}}}
