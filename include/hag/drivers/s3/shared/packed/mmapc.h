//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/majapcnt.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        MinorMajorAxisPixelCounts = 0x8148,
    };
}}

namespace MMIO { namespace Packed { namespace MinorMajorAxisPixelCounts
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::MinorMajorAxisPixelCounts).ToPointer<uint32_t>();
    }

    inline void Write(MajorAxisPixelCount_t majorAxisPixelCount, MinorAxisPixelCount_t minorAxisPixelCount)
    {
            Get() = (uint32_t(majorAxisPixelCount) << 16) | minorAxisPixelCount;
    }
}}}

}}}
