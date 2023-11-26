//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/vga/sqrc/index.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    SequencerData = 0x3C5,                                  //SEQ_DATA
};

}

typedef uint8_t SequencerData_t;

namespace SequencerData
{
    
    inline SequencerData_t Read()
    {
        return SequencerData_t(SYS_ReadPortByte(Register::SequencerData));
    }

    inline void Write(SequencerData_t value)
    {
        SYS_WritePortByte(Register::SequencerData, value);
    }
    
    inline void Write(Sequencer::Register_t sequenceIndex, SequencerData_t value)
    {
        SYS_WritePortBytes(Register::SequencerIndex, sequenceIndex, value);
    }

}

}}