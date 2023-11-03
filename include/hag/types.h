//Copyright 2023-Present riplin

#pragma once

#ifndef NULL
#define NULL 0
#endif

//These are defined to clearly communicate the number of bits that are expected.
typedef unsigned char uint1_t;
typedef unsigned char uint2_t;
typedef unsigned char uint3_t;
typedef unsigned char uint4_t;
typedef unsigned char uint5_t;
typedef unsigned char uint6_t;
typedef unsigned char uint7_t;
typedef unsigned char uint8_t;
typedef unsigned short uint9_t;
typedef unsigned short uint10_t;
typedef unsigned short uint11_t;
typedef unsigned short uint12_t;
typedef unsigned short uint13_t;
typedef unsigned short uint14_t;
typedef unsigned short uint15_t;
typedef unsigned short uint16_t;
typedef unsigned int uint17_t;
typedef unsigned int uint18_t;
typedef unsigned int uint19_t;
typedef unsigned int uint20_t;

typedef unsigned int uint32_t;
//    typedef long long int uint64_t;

struct FARPointer
{
    inline FARPointer(uint16_t seg, uint16_t offs) : Offset(offs), Segment(seg){}
    inline bool IsNull() { return (Offset == 0x0000) && (Segment == 0x0000);}

    template<typename T> T*ToPointer() { return (T*)((uint32_t(Segment) << 4) + Offset);}

    uint16_t Offset;
    uint16_t Segment;
};