#include "y/calculus/exact-shift.hpp"

namespace Yttrium
{
    namespace Core
    {

#define Y_Exact_Shift(i) case (One << i): return i;


        unsigned ExactShift<uint8_t>:: Of(const uint8_t x) noexcept
        {
            static const uint8_t One = 1;

            switch(x)
            {
                    Y_Exact_Shift(0);
                    Y_Exact_Shift(1);
                    Y_Exact_Shift(2);
                    Y_Exact_Shift(3);
                    Y_Exact_Shift(4);
                    Y_Exact_Shift(5);
                    Y_Exact_Shift(6);
                    Y_Exact_Shift(7);
                default:
                    break;
            }
            return sizeof(One);
        }


        unsigned ExactShift<uint16_t>:: Of(const uint16_t x) noexcept
        {
            static const uint8_t One = 1;

            switch(x)
            {
                    Y_Exact_Shift(0);
                    Y_Exact_Shift(1);
                    Y_Exact_Shift(2);
                    Y_Exact_Shift(3);
                    Y_Exact_Shift(4);
                    Y_Exact_Shift(5);
                    Y_Exact_Shift(6);
                    Y_Exact_Shift(7);
                    Y_Exact_Shift(8);
                    Y_Exact_Shift(9);
                    Y_Exact_Shift(10);
                    Y_Exact_Shift(11);
                    Y_Exact_Shift(12);
                    Y_Exact_Shift(13);
                    Y_Exact_Shift(14);
                    Y_Exact_Shift(15);
                default:
                    break;
            }
            return sizeof(One);
        }

        unsigned ExactShift<uint32_t>:: Of(const uint32_t x) noexcept
        {
            static const uint32_t One = 1;

            switch(x)
            {
                    Y_Exact_Shift(0);
                    Y_Exact_Shift(1);
                    Y_Exact_Shift(2);
                    Y_Exact_Shift(3);
                    Y_Exact_Shift(4);
                    Y_Exact_Shift(5);
                    Y_Exact_Shift(6);
                    Y_Exact_Shift(7);
                    Y_Exact_Shift(8);
                    Y_Exact_Shift(9);
                    Y_Exact_Shift(10);
                    Y_Exact_Shift(11);
                    Y_Exact_Shift(12);
                    Y_Exact_Shift(13);
                    Y_Exact_Shift(14);
                    Y_Exact_Shift(15);
                    Y_Exact_Shift(16);
                    Y_Exact_Shift(17);
                    Y_Exact_Shift(18);
                    Y_Exact_Shift(19);
                    Y_Exact_Shift(20);
                    Y_Exact_Shift(21);
                    Y_Exact_Shift(22);
                    Y_Exact_Shift(23);
                    Y_Exact_Shift(24);
                    Y_Exact_Shift(25);
                    Y_Exact_Shift(26);
                    Y_Exact_Shift(27);
                    Y_Exact_Shift(28);
                    Y_Exact_Shift(29);
                    Y_Exact_Shift(30);
                    Y_Exact_Shift(31);
                default:
                    break;
            }
            return sizeof(One);
        }




        unsigned ExactShift<uint64_t>:: Of(const uint64_t x) noexcept
        {
            static const uint64_t One = 1;

            switch(x)
            {
                    Y_Exact_Shift(0);
                    Y_Exact_Shift(1);
                    Y_Exact_Shift(2);
                    Y_Exact_Shift(3);
                    Y_Exact_Shift(4);
                    Y_Exact_Shift(5);
                    Y_Exact_Shift(6);
                    Y_Exact_Shift(7);
                    Y_Exact_Shift(8);
                    Y_Exact_Shift(9);
                    Y_Exact_Shift(10);
                    Y_Exact_Shift(11);
                    Y_Exact_Shift(12);
                    Y_Exact_Shift(13);
                    Y_Exact_Shift(14);
                    Y_Exact_Shift(15);
                    Y_Exact_Shift(16);
                    Y_Exact_Shift(17);
                    Y_Exact_Shift(18);
                    Y_Exact_Shift(19);
                    Y_Exact_Shift(20);
                    Y_Exact_Shift(21);
                    Y_Exact_Shift(22);
                    Y_Exact_Shift(23);
                    Y_Exact_Shift(24);
                    Y_Exact_Shift(25);
                    Y_Exact_Shift(26);
                    Y_Exact_Shift(27);
                    Y_Exact_Shift(28);
                    Y_Exact_Shift(29);
                    Y_Exact_Shift(30);
                    Y_Exact_Shift(31);
                    Y_Exact_Shift(32);
                    Y_Exact_Shift(33);
                    Y_Exact_Shift(34);
                    Y_Exact_Shift(35);
                    Y_Exact_Shift(36);
                    Y_Exact_Shift(37);
                    Y_Exact_Shift(38);
                    Y_Exact_Shift(39);
                    Y_Exact_Shift(40);
                    Y_Exact_Shift(41);
                    Y_Exact_Shift(42);
                    Y_Exact_Shift(43);
                    Y_Exact_Shift(44);
                    Y_Exact_Shift(45);
                    Y_Exact_Shift(46);
                    Y_Exact_Shift(47);
                    Y_Exact_Shift(48);
                    Y_Exact_Shift(49);
                    Y_Exact_Shift(50);
                    Y_Exact_Shift(51);
                    Y_Exact_Shift(52);
                    Y_Exact_Shift(53);
                    Y_Exact_Shift(54);
                    Y_Exact_Shift(55);
                    Y_Exact_Shift(56);
                    Y_Exact_Shift(57);
                    Y_Exact_Shift(58);
                    Y_Exact_Shift(59);
                    Y_Exact_Shift(60);
                    Y_Exact_Shift(61);
                    Y_Exact_Shift(62);
                    Y_Exact_Shift(63);
                default:
                    break;
            }
            return sizeof(One);
        }
    }

}

