//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/index.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    GraphicsControllerData = 0x3CF,                         //GRC_DATA
};

}

typedef uint8_t GraphicsControllerData_t;

namespace GraphicsControllerData
{
    inline GraphicsControllerData_t Read()
    {
        return GraphicsControllerData_t(SYS_ReadPortByte(Register::GraphicsControllerData));
    }

    inline void Write(GraphicsControllerData_t value)
    {
        SYS_WritePortByte(Register::GraphicsControllerData, value);
    }

    inline void Write(GraphicsController::Register_t gfxIndex, GraphicsControllerData_t value)
    {
        SYS_WritePortBytes(Register::GraphicsControllerIndex, gfxIndex, value);
    }
}

}}
