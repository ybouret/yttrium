
#include "y/hashing/ibj64.hpp"
#include <cstdio>

namespace Yttrium
{
    namespace Hashing
    {

        uint32_t IBJ32(uint32_t a) noexcept
        {
            a = (a+0x7ed55d16) + (a<<12);
            a = (a^0xc761c23c) ^ (a>>19);
            a = (a+0x165667b1) + (a<<5);
            a = (a+0xd3a2646c) ^ (a<<9);
            a = (a+0xfd7046c5) + (a<<3);
            a = (a^0xb55a4f09) ^ (a>>16);
            return a;
        }

        const char * const IBJ64:: CallSign = "Hashing::IBJ64";

        IBJ64::  IBJ64() noexcept {}
        IBJ64:: ~IBJ64() noexcept {}

        const char * IBJ64:: callSign() const noexcept { return CallSign; }

        void    IBJ64::operator()(uint32_t &lword, uint32_t &rword) const noexcept
        {
            for(size_t iter=0;iter<4;++iter)
            {
                const uint32_t swp = rword;
                rword = IBJ32(rword) ^ lword;
                lword = swp;
            }
        }


    }
}

