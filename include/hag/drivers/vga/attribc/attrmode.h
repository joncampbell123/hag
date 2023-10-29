//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/data.h>

namespace Hag { namespace VGA { namespace AttributeController
{

typedef uint8_t AttributeMode_t;

//The contents of this register controls the attribute mode of the display function.

namespace AttributeMode
{
    enum //Mask
    {
        SelectGraphicsMode = 0x01,          //0 = Selects text attribute control mode.
                                            //1 = Selects graphics control mode.
        SelectMonochromeAttributes = 0x02,  //0 = Selects color display text attributes.
                                            //1 = Selects monochrome display text attributes.
        EnableLineGraphics = 0x04,          //0 = The ninth dot of a text character (bit 0 of SR1 = 0) is the same
                                            //    as the background.
                                            //1 = Special line graphics character codes enabled.
                                            //When this bit is set to 1, it forces the ninth dot of a line graphics
                                            //character to be identi- cal to the eighth dot of the character. 
                                            //The line graphics character codes are COH through DFH. 
                                            //For other characters, the ninth dot is the same as the background.
        EnableBlinking = 0x08,              //0 = Selects the background intensity for the text attribute input.
                                            //1 = Selects blink attribute in text modes.
                                            //This bit must also be set to 1 for blinking graphics modes.
                                            //The blinking counter is op- erated by the vertical retrace counter
                                            //(VRTC) input. It divides the VRTC input by 32. The blinking rates are
                                            //ON for 16 VRTC clocks and OFF for 16 VRTC clocks. In the graphics mode,
                                            //when blink is activated, the most significant color bit (bit 3) for each
                                            //dot is inverted alternately, thus allowing two different colors to be
                                            //displayed for 16 VRTC clocks each.
                                            //When the cursor is displayed in the text mode, it is blinked at a rate of
                                            //ON for 8 VRTC clocks and OFF for 8 VRTC clocks (period by 16 frames). The
                                            //displayed characters are independently blinked at the rate of 32 frames as above.
        TopPanningEnable = 0x20,            //0 = Line compare has no effect on the output of the pixel panning register.
                                            //1 = Forces the output of the pixel panning register to 0 after matching line
                                            //compare until VSYNC occurs in the CRT controller. At the top of screen the 
                                            //output of the Pixel Panning register returns to its programmed value. This
                                            //bit allows a top portion of a split screen to be panned.
        Select256ColorMode = 0x40,          //0 = 4 bits of video (translated to 6 bits by the palette) are output
                                            //every internal dot-clock cycle.
                                            //1 = Two 4-bit sets of video data are assembled to generate a-bit video data
                                            //at half the frequency of the internal dot-clock.
        SelectV54 = 0x80                    //0 = In VGA, mode, bits 5-4 of video output are generated by the attribute
                                            //palette registers. Bits 7-6 of video output are always generated by bits 
                                            //3-2 of AR14.
                                            //1 = Bits 5-4 of video output are generated by bits 1-0 of AR14.
    };

    namespace Shift
    {
        enum
        {
            SelectGraphicsMode = 0x00,
            SelectMonochromeAttributes = 0x01,
            EnableLineGraphics = 0x02,
            EnableBlinking = 0x03,
            TopPanningEnable = 0x05,
            Select256ColorMode = 0x06,
            SelectV54 = 0x07
        };
    }

    inline AttributeMode_t Read()
    {
        AttributeControllerIndex::Write(AttributeControllerRegister::AttributeMode);
        return AttributeMode_t(AttributeControllerData::Read());
    }
    
    inline void Write(AttributeMode_t value)
    {
        AttributeControllerData::Write(AttributeControllerRegister::AttributeMode, AttributeControllerData_t(value));
    }

    inline uint1_t GetSelectGraphicsMode(AttributeMode_t value)
    {
        return (value & SelectGraphicsMode) >> Shift::SelectGraphicsMode;
    }

    inline uint1_t GetSelectMonochromeAttributes(AttributeMode_t value)
    {
        return (value & SelectMonochromeAttributes) >> Shift::SelectMonochromeAttributes;
    }

    inline uint1_t GetEnableLineGraphics(AttributeMode_t value)
    {
        return (value & EnableLineGraphics) >> Shift::EnableLineGraphics;
    }

    inline uint1_t GetEnableBlinking(AttributeMode_t value)
    {
        return (value & EnableBlinking) >> Shift::EnableBlinking;
    }

    inline uint1_t GetTopPanningEnable(AttributeMode_t value)
    {
        return (value & TopPanningEnable) >> Shift::TopPanningEnable;
    }

    inline uint1_t GetSelect256ColorMode(AttributeMode_t value)
    {
        return (value & Select256ColorMode) >> Shift::Select256ColorMode;
    }

    inline uint1_t GetSelectV54(AttributeMode_t value)
    {
        return (value & SelectV54) >> Shift::SelectV54;
    }

    inline AttributeMode_t SetSelectGraphicsMode(uint1_t value)
    {
        return AttributeMode_t(value << Shift::SelectGraphicsMode) & SelectGraphicsMode;
    }

    inline AttributeMode_t SetSelectMonochromeAttributes(uint1_t value)
    {
        return AttributeMode_t(value << Shift::SelectMonochromeAttributes) & SelectMonochromeAttributes;
    }

    inline AttributeMode_t SetEnableLineGraphics(uint1_t value)
    {
        return AttributeMode_t(value << Shift::EnableLineGraphics) & EnableLineGraphics;
    }

    inline AttributeMode_t SetEnableBlinking(uint1_t value)
    {
        return AttributeMode_t(value << Shift::EnableBlinking) & EnableBlinking;
    }

    inline AttributeMode_t SetTopPanningEnable(uint1_t value)
    {
        return AttributeMode_t(value << Shift::TopPanningEnable) & TopPanningEnable;
    }

    inline AttributeMode_t SetSelect256ColorMode(uint1_t value)
    {
        return AttributeMode_t(value << Shift::Select256ColorMode) & Select256ColorMode;
    }

    inline AttributeMode_t SetSelectV54(uint1_t value)
    {
        return AttributeMode_t(value << Shift::SelectV54) & SelectV54;
    }
}

}}}
