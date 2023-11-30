//Copyright 2023-Present riplin

#include <hag/system/bda.h>
#include <hag/drivers/s3/trio32/data.h>

namespace Hag { namespace S3 { namespace Trio32 { namespace Data
{

VGA::VideoParameters* LegacyVideoModesV1[] =
{
    &Shared::Data::LegacyMode00v1,
    &Shared::Data::LegacyMode00v1,
    &Shared::Data::LegacyMode02v1,
    &Shared::Data::LegacyMode02v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode06v1,
    &Shared::Data::LegacyMode07v1,
    &Shared::Data::LegacyMode08v1,
    &Shared::Data::LegacyMode09v1,
    &Shared::Data::LegacyMode0Av1,
    &Shared::Data::LegacyMode0Bv1,
    &Shared::Data::LegacyMode0Cv1,
    &Shared::Data::LegacyMode0Dv1,
    &Shared::Data::LegacyMode0Ev1,
    &Shared::Data::LegacyMode0FTrio32,
    &Shared::Data::LegacyMode10v1,
    &Shared::Data::LegacyMode11v1,
    &Shared::Data::LegacyMode12v1,
    &Shared::Data::LegacyMode13v1
};

VGA::VideoParameters* LegacyVideoModesV2[] =
{
    &Shared::Data::LegacyMode00v2,
    &Shared::Data::LegacyMode00v2,
    &Shared::Data::LegacyMode02v2,
    &Shared::Data::LegacyMode02v2,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode06v1,
    &Shared::Data::LegacyMode07v1,
    &Shared::Data::LegacyMode08v1,
    &Shared::Data::LegacyMode09v1,
    &Shared::Data::LegacyMode0Av1,
    &Shared::Data::LegacyMode0Bv1,
    &Shared::Data::LegacyMode0Cv1,
    &Shared::Data::LegacyMode0Dv1,
    &Shared::Data::LegacyMode0Ev1,
    &Shared::Data::LegacyMode0FTrio32,
    &Shared::Data::LegacyMode10v1,
    &Shared::Data::LegacyMode11v1,
    &Shared::Data::LegacyMode12v1,
    &Shared::Data::LegacyMode13v1
};

VGA::VideoParameters* LegacyVideoModesV3[] =
{
    &Shared::Data::LegacyMode00v3,
    &Shared::Data::LegacyMode00v3,
    &Shared::Data::LegacyMode02v3,
    &Shared::Data::LegacyMode02v3,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode06v1,
    &Shared::Data::LegacyMode07v3,
    &Shared::Data::LegacyMode08v1,
    &Shared::Data::LegacyMode09v1,
    &Shared::Data::LegacyMode0Av1,
    &Shared::Data::LegacyMode0Bv1,
    &Shared::Data::LegacyMode0Cv1,
    &Shared::Data::LegacyMode0Dv1,
    &Shared::Data::LegacyMode0Ev1,
    &Shared::Data::LegacyMode0FTrio32,
    &Shared::Data::LegacyMode10v1,
    &Shared::Data::LegacyMode11v1,
    &Shared::Data::LegacyMode12v1,
    &Shared::Data::LegacyMode13v1
};

VGA::VideoParameters* VesaResolutions[] =       //This table is only used to lookup the applicable refresh rate in a rather clumsy manner
{
    &Shared::Data::VESAVideoParameters132x43,
    &Shared::Data::VESAVideoParameters132x25,
    &Shared::Data::VESAVideoParameters640x400,
    &Shared::Data::VESAVideoParameters640x480,
    &Shared::Data::VESAVideoParameters800x600,
    &Shared::Data::VESAVideoParameters1024x768,
    &Shared::Data::VESAVideoParameters1280x1024,
    &Shared::Data::VESAVideoParameters1152x864,
};

Shared::VESAResolutionVariant ModeData132x43x8[] =
{
    {
        Shared::Data::VESAVideoParameters132x43.CRTCRegisters,
        0x80, 0x02, 0x00, 0x08, 0x08, 0x38, 0x38, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData132x25x8[] =
{
    {
        Shared::Data::VESAVideoParameters132x25.CRTCRegisters,
        0x80, 0x02, 0x00, 0x08, 0x08, 0x38, 0x38, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData640x400x8[] =
{
    {
        Shared::Data::VESAVideoParameters640x400.CRTCRegisters,
        0x80, 0x40, 0x40, 0x50, 0x50, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData640x480x8[] =
{
    {
        Shared::Data::CRTData640x480x2v1,
        0x02, 0x4B, 0x40, 0x50, 0x50, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData640x480x1v1,
        0x01, 0x4B, 0x40, 0x38, 0x38, 0x0F, 0x0F, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters640x480.CRTCRegisters,
        0x80, 0x40, 0x40, 0x68, 0x68, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x4[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        0x03, 0x10, 0x02, 0x18, 0x18, 0xFF, 0xFF, 0x08, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x2v1,
        0x02, 0x04, 0x02, 0x18, 0x18, 0xFF, 0xFF, 0x08, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x1v1,
        0x01, 0x42, 0x02, 0x28, 0x28, 0xFF, 0xFF, 0x08, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        0x80, 0x46, 0x00, 0xF8, 0xF8, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x8[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        0x03, 0x10, 0x82, 0x18, 0x18, 0xFF, 0xFF, 0x08, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x2v1,
        0x02, 0x04, 0x82, 0x18, 0x18, 0xFF, 0xFF, 0x08, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x1v1,
        0x01, 0x42, 0x82, 0x28, 0x28, 0xFF, 0xFF, 0x08, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        0x80, 0x46, 0x80, 0xF8, 0xF8, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x4[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        0x04, 0x0A, 0x02, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x01
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        0x03, 0x0E, 0x00, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x01
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        0x02, 0x0D, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        0x80, 0x27, 0x00, 0x20, 0x20, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x8[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        0x04, 0x0A, 0x02, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x01
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        0x03, 0x0E, 0x00, 0x38, 0x38, 0x6B, 0x6B, 0x00, 0x00, 0x01
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        0x02, 0x0D, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        0x80, 0x27, 0x00, 0x20, 0x20, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x4[] =
{
    {
        Shared::Data::VESAVideoParameters1280x1024.CRTCRegisters,
        0x82, 0x2E, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x8[] =
{
    {
        Shared::Data::CRTData1280x1024x4,
        0x04, 0x0C, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x55, 0x10
    },
    {
        Shared::Data::VESAVideoParameters1280x1024.CRTCRegisters,
        0x82, 0x2E, 0xC0, 0x40, 0x40, 0xFF, 0xFF, 0x00, 0x00, 0x01
    }
};

Shared::VESAResolutionVariant ModeData640x480x16[] =
{
    {
        Shared::Data::CRTData640x480x2v2,
        0x02, 0x0B, 0x50, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x50
    },
    {
        Shared::Data::CRTData640x480x1v2,
        0x01, 0x0B, 0x50, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x50
    },
    {
        Shared::Data::CRTData640x480x0,
        0x80, 0x00, 0x50, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x50
    }
};

Shared::VESAResolutionVariant ModeData640x480x32[] =
{
    {
        Shared::Data::VESAVideoParameters640x480.CRTCRegisters,
        0x80, 0x00, 0x70, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0xD0
    }
};

Shared::VESAResolutionVariant ModeData800x600x16[] =
{
    {
        Shared::Data::CRTData800x600x3v2,
        0x03, 0x10, 0x92, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x50
    },
    {
        Shared::Data::CRTData800x600x2v2,
        0x02, 0x04, 0x92, 0x08, 0x08, 0x5A, 0x5A, 0x01, 0x00, 0x50
    },
    {
        Shared::Data::CRTData800x600x1v2,
        0x81, 0x02, 0x92, 0x08, 0x08, 0xFF, 0xFF, 0x01, 0x00, 0x50
    }
};

Shared::VESAResolutionVariant ModeData1024x768x16[] =
{
    {
        Shared::Data::CRTData1024x768x0,
        0x80, 0x27, 0x10, 0x08, 0x08, 0xFF, 0xFF, 0x75, 0x00, 0x50
    }
};

Shared::VESAResolutionVariant ModeData640x480x8xOEM[] =
{
    {
        Shared::Data::CRTData640x480x2v1,
        0x02, 0x4B, 0x00, 0x50, 0x50, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData640x480x1v1,
        0x01, 0x4B, 0x00, 0x38, 0x38, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters640x480.CRTCRegisters,
        0x80, 0x40, 0x00, 0x68, 0x68, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x4xOEM[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        0x03, 0x10, 0x02, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x2v1,
        0x02, 0x04, 0x02, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x1v1,
        0x01, 0x42, 0x02, 0x28, 0x28, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        0x80, 0x46, 0x00, 0xF8, 0xF8, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x8xOEM[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        0x03, 0x10, 0x00, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x2v1,
        0x02, 0x04, 0x00, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData800x600x1v1,
        0x01, 0x42, 0x00, 0x28, 0x28, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        0x80, 0x46, 0x00, 0xF8, 0xF8, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x4xOEM[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        0x04, 0x0A, 0x02, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        0x03, 0x0E, 0x00, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        0x02, 0x0D, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        0x80, 0x27, 0x00, 0x20, 0x20, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x8xOEM[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        0x04, 0x0A, 0x00, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        0x03, 0x0E, 0x00, 0x38, 0x38, 0x6B, 0x6B, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        0x02, 0x0D, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00
    },
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        0x80, 0x27, 0x00, 0x20, 0x20, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1152x864x8xOEM[] =
{
    {
        Shared::Data::VESAVideoParameters1152x864.CRTCRegisters,
        0x84, 0x0A, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x4xOEM[] =
{
    {
        Shared::Data::CRTData1280x1024x6,
        0x06, 0x08, 0x02, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x15, 0x10
    },
    {
        Shared::Data::CRTData1280x1024x5,
        0x05, 0x09, 0x00, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x55, 0x10
    },
    {
        Shared::Data::CRTData1280x1024x4,
        0x04, 0x0C, 0x00, 0x08, 0x08, 0xFF, 0xFF, 0x00, 0x55, 0x10
    },
    {
        Shared::Data::VESAVideoParameters1280x1024.CRTCRegisters,
        0x82, 0x2E, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x01
    }
};

Shared::VESAResolutionVariant ModeData640x480x24xOEM[] =
{
    {
        Shared::Data::CRTData640x480x0v2,
        0x80, 0x0E, 0x00, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x01
    }
};

Shared::VESAVideoModeData VesaVideoModes[] = 
{
    {
        &Shared::Data::VESAVideoParameters132x43,
        ModeData132x43x8,
        &Shared::Data::ModeInfo_54_T_132x43x4bpp,
        0x54,
        0x03,
        0x02,
        0x00,
        0x02
    },
    {
        &Shared::Data::VESAVideoParameters132x25,
        ModeData132x25x8,
        &Shared::Data::ModeInfo_55_T_132x25x4bpp,
        0x55,
        0x33,
        0x02,
        0x00,
        0x02
    },
    {
        &Shared::Data::VESAVideoParameters640x400,
        ModeData640x400x8,
        &Shared::Data::ModeInfo_68_G_640x400x8bpp,
        0x68,
        0x36,
        0x03,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x8,
        &Shared::Data::ModeInfo_69_G_640x480x8bpp,
        0x69,
        0x36,
        0x03,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x4,
        &Shared::Data::ModeInfo_6A_G_800x600x4bpp,
        0x6A,
        0x02,
        0x06,
        0x00,
        0x06
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x8,
        &Shared::Data::ModeInfo_6B_G_800x600x8bpp,
        0x6B,
        0x06,
        0x03,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x4,
        &Shared::Data::ModeInfo_6C_1024x768x4bpp,
        0x6C,
        0x32,
        0x06,
        0x00,
        0x06
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x8,
        &Shared::Data::ModeInfo_6D_1024x768x8bpp,
        0x6D,
        0x36,
        0x07,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x4,
        &Shared::Data::ModeInfo_6E_1280x1024x4bpp,
        0x6E,
        0x32,
        0x06,
        0x00,
        0x06
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x8,
        &Shared::Data::ModeInfo_6F_1280x1024x8bpp,
        0x6F,
        0x36,
        0x03,
        0x05,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x16,
        &Shared::Data::ModeInfo_70_640x480x15bpp,
        0x70,
        0x36,
        0x03,
        0x01,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x16,
        &Shared::Data::ModeInfo_71_640x480x16bpp,
        0x71,
        0x36,
        0x03,
        0x02,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x32,
        &Shared::Data::ModeInfo_72_640x480x32bpp,
        0x72,
        0x36,
        0x03,
        0x04,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x16,
        &Shared::Data::ModeInfo_73_800x600x15bpp,
        0x73,
        0x06,
        0x03,
        0x01,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x16,
        &Shared::Data::ModeInfo_74_800x600x16bpp,
        0x74,
        0x06,
        0x03,
        0x02,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x16,
        &Shared::Data::ModeInfo_76_1024x768x15bpp,
        0x76,
        0x36,
        0x03,
        0x01,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x16,
        &Shared::Data::ModeInfo_77_1024x768x16bpp,
        0x77,
        0x36,
        0x03,
        0x02,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x8xOEM,
        &Shared::Data::ModeInfo_49_640x480x8bpp,
        0x49,
        0x36,
        0x03,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x4xOEM,
        &Shared::Data::ModeInfo_4A_800x600x4bpp,
        0x4A,
        0x02,
        0x07,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x8xOEM,
        &Shared::Data::ModeInfo_4B_800x600x8bpp,
        0x4B,
        0x06,
        0x07,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x4xOEM,
        &Shared::Data::ModeInfo_4C_1024x768x4bpp,
        0x4C,
        0x32,
        0x07,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x8xOEM,
        &Shared::Data::ModeInfo_4D_1024x768x8bpp,
        0x4D,
        0x36,
        0x07,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1152x864,
        ModeData1152x864x8xOEM,
        &Shared::Data::ModeInfo_4E_1152x864x8bpp,
        0x4E,
        0x36,
        0x07,
        0x00,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x4xOEM,
        &Shared::Data::ModeInfo_4F_1280x1024x4bpp,
        0x4F,
        0x32,
        0x07,
        0x05,
        0x0E
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x24xOEM,
        &Shared::Data::ModeInfo_51_640x480x24bpp,
        0x51,
        0x36,
        0x03,
        0x03,
        0x0E
    }
};

int VesaVideoModesCount = sizeof(VesaVideoModes) / sizeof(Shared::VESAVideoModeData);

//;DCLK High/Low values - Trio32 data
uint8_t ClockData[] =
{
    0x28, 0x61, //00h
    0x55, 0x49, //01h
    0x2B, 0x42, //02h
    0x7C, 0x24, //03h
    0x1A, 0x40, //04h
    0x29, 0x22, //05h
    0x63, 0x48, //06h
    0x30, 0x42, //07h
    0x2D, 0x03, //08h
    0x50, 0x07, //09h
    0x2B, 0x22, //0Ah
    0x33, 0x61, //0Bh
    0x2D, 0x21, //0Ch
    0x47, 0x42, //0Dh
    0x13, 0x20, //0Eh
    0x26, 0x21, //0Fh
    0x51, 0x44  //10h
};

uint8_t SequenceInitData[] =
{
    0x04,
    0x08, 0x06,
    0x09, 0x06,
    0x0B, 0x00,
    0x14, 0x00
};

Shared::FirmwareFlag_t FirmwareFlags = Shared::FirmwareFlag::Color | Shared::FirmwareFlag::Unknown3;

}}}}