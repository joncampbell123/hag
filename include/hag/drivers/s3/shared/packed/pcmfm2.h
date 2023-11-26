//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        PixelControlAndMultiFunctionMisc2 = 0x8140,
    };
}}

namespace MMIO { namespace Packed { namespace PixelControlAndMultiFunctionMisc2
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::PixelControlAndMultiFunctionMisc2).ToPointer<uint32_t>();
    }

    inline void Write(MultifunctionControlMiscellaneous2_t multiFunctionControlMisc2, PixelControl_t pixelControl)
    {
            Get() = (uint32_t(multiFunctionControlMisc2) << 16) | pixelControl;
    }
}}}

}}}
