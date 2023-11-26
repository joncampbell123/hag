//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{

enum
{
    CurrentXPosition = 0x86E8,                              //CUR_X
};

}

typedef uint16_t CurrentXPosition_t;

namespace CurrentXPosition
{

    inline CurrentXPosition_t Read()
    {
        return CurrentXPosition_t(SYS_ReadPortShort(Register::CurrentXPosition));
    }

    inline void Write(CurrentXPosition_t value)
    {
        SYS_WritePortShort(Register::CurrentXPosition, value);
    }
}

namespace MMIO
{

    namespace CurrentXPosition
    {
        inline CurrentXPosition_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentXPosition).
                ToPointer<CurrentXPosition_t>(sizeof(CurrentXPosition_t));
        }

        inline void Write(CurrentXPosition_t value)
        {
            *FARPointer(0xA000, Register::CurrentXPosition).
                ToPointer<CurrentXPosition_t>(sizeof(CurrentXPosition_t)) = value;
        }
    }

}

}}}
