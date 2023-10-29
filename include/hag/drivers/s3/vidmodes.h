//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/vidmodes.h>
#include <hag/vesa/vidmodes.h>

namespace Hag { namespace S3
{

typedef Vesa::VideoMode_t VesaVideoMode_t;

namespace VesaVideoMode
{
    using namespace Vesa::VideoMode;

    enum
    {
        P640x480x256C = 0x201,
        P800x600x16C = 0x202,
        P800x600x256C = 0x203,
        P1024x768x16C = 0x204,
        P1024x768x256C = 0x205,
        P1152x864x256C = 0x207,
        P1280x1024x16C = 0x208,
        P640x400x16M = 0x213,
    };
}


typedef VGA::VideoMode_t VideoMode_t;
namespace VideoMode
{
    using namespace VGA::VideoMode;

    enum
    {
        Invalid = 0xFF, //Used when converting Vesa modes to legacy modes and the mode is not supported
        P640x480x256C = 0x49, //Proprietary VESA Mode 0x201
        P800x600x16C = 0x4A, //Proprietary VESA Mode 0x202
        P800x600x256C = 0x4B, //Proprietary VESA Mode 0x203
        P1024x768x16C = 0x4C, //Proprietary VESA Mode 0x204
        P1024x768x256C = 0x4D, //Proprietary VESA Mode 0x205
        P1152x864x256C = 0x4E, //Proprietary VESA Mode 0x207
        P1280x1024x16C = 0x4F, //Proprietary VESA Mode 0x208
        P640x400x16M = 0x52, //Proprietary VESA Mode 0x213
        T132x43x16C = 0x54, //VESA Mode 0x10A
        T132x25x16C = 0x55, //VESA Mode 0x109
        G640x400x256C = 0x68, //VESA Mode 0x100
        G640x480x256C = 0x69, //VESA Mode 0x101
        G800x600x16C = 0x6A, //VESA Mode 0x102
        G800x600x256C = 0x6B, //VESA Mode 0x103
        G1024x768x16C = 0x6C, //VESA Mode 0x104
        G1024x768x256C = 0x6D, //VESA Mode 0x105
        G1280x1024x16C = 0x6E, //VESA Mode 0x106
        G1280x1024x256C = 0x6F, //VESA Mode 0x107
        G640x480x32K = 0x70, //VESA Mode 0x110
        G640x480x64K = 0x71, //VESA Mode 0x111
        G640x480x16M = 0x72, //VESA Mode 0x112
        G800x600x32K = 0x73, //VESA Mode 0x113
        G800x600x64K = 0x74, //VESA Mode 0x114
        G800x600x16M = 0x75, //VESA Mode 0x115
        G1024x768x32K = 0x76, //VESA Mode 0x116
        G1024x768x64K = 0x77, //VESA Mode 0x117
        G1024x768x16M = 0x78, //VESA Mode 0x118
        G1280x1024x32K = 0x79, //VESA Mode 0x119
        G1280x1024x64K = 0x7A, //VESA Mode 0x11A
        P1600x1200x256C = 0x7C, //VESA Mode 0x120
    };
}

}}
