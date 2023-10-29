//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

//This register should contain the same value as the upper byte of the PCI Device ID (Index 0x00) register.

typedef uint8_t DeviceIDHigh_t;

namespace DeviceIDHigh
{

    inline DeviceIDHigh_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::DeviceIDHigh);
        return DeviceIDHigh_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}}}
