#include "y/system/endian.hpp"

namespace Yttrium
{
    Endianness::Type Endianness:: Detect() noexcept
    {
        union
        {
            uint32_t value;
            uint8_t  data[sizeof(uint32_t)];
        } number = { 0 };

        number.data[0] = 0x00;
        number.data[1] = 0x01;
        number.data[2] = 0x02;
        number.data[3] = 0x03;

        switch (number.value)
        {
            case uint32_t(0x00010203): return Big;
            case uint32_t(0x03020100): return Little;
            case uint32_t(0x02030001): return BigWord;
            case uint32_t(0x01000302): return LittleWord;
            default:
                break;
        }
        return Unknown;
    }


#define Y_ENDIAN(T) case T: return #T

    const char * Endianness:: Text(const Type t) noexcept
    {
        switch(t)
        {
                Y_ENDIAN(Big);
                Y_ENDIAN(Little);
                Y_ENDIAN(BigWord);
                Y_ENDIAN(LittleWord);
            default:
                break;
        }
        return Core::Unknown;
    }

    const char * const Endianness:: CallSign = "Endianness";


    uint8_t Endianness:: hton(const uint8_t x) const noexcept { return x; }
    

    Endianness:: Endianness() :
    Singleton<Endianness>(),
    type( Detect() )
    {
    }

    Endianness:: ~Endianness() noexcept
    {
        
    }




}

