#include "y/text/base64/encode.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/type/div.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Base64
    {
        const char Encode::Table[64] =
        {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '+', '/'
        };

        static const uint8_t maskLo2 = 0x03;
        static const uint8_t maskHi6 = 0xfc;
        static const uint8_t maskLo4 = 0x0f;
        static const uint8_t maskHi4 = 0xf0;
        static const uint8_t maskHi2 = 0xc0;
        static const uint8_t maskLo6 = 0x3f;

        void Encode:: ShowInfo()
        {
            std::cerr << "maskLo2 : 0x" << Hexadecimal(maskLo2) << std::endl;
            std::cerr << "maskHi6 : 0x" << Hexadecimal(maskHi6) << std::endl;
            std::cerr << "maskLo4 : 0x" << Hexadecimal(maskLo4) << std::endl;
            std::cerr << "maskHi4 : 0x" << Hexadecimal(maskHi4) << std::endl;
            std::cerr << "maskHi2 : 0x" << Hexadecimal(maskHi2) << std::endl;
            std::cerr << "maskLo6 : 0x" << Hexadecimal(maskLo6) << std::endl;

        }

        size_t Encode:: _1(char         *output,
                           const uint8_t code,
                           const bool    pad) noexcept
        {
            assert(0!=output);
            const uint8_t b0 = (code&maskHi6) >> 2; assert(b0<64); output[0] = Table[b0];
            const uint8_t b1 = (code&maskLo2) << 4; assert(b1<64); output[1] = Table[b1];
            if( pad )
            {
                output[2] = Padding;
                output[3] = Padding;
                return 4;
            }
            else
            {
                return 2;
            }
        }

        size_t Encode:: _2(char * output, const uint8_t c0, const uint8_t c1, const bool pad) noexcept
        {
            assert(0!=output);
            const uint8_t b0 = (c0&maskHi6) >> 2;                            assert(b0<64); output[0] = Table[b0];
            const uint8_t b1 = ((c0&maskLo2) << 4) | ( (c1&maskHi4) >> 4);   assert(b1<64); output[1] = Table[b1];
            const uint8_t b2 = (c1&maskLo4) << 2;                            assert(b2<64); output[2] = Table[b2];
            if( pad )
            {
                output[3] = Padding;
                return 4;
            }
            else
            {
                return 3;
            }
        }

        size_t Encode:: _3(char *output, const uint8_t c0, const uint8_t c1, const uint8_t c2) noexcept
        {

            const uint8_t b0 = (c0&maskHi6) >> 2;                            assert(b0<64); output[0] = Table[b0];
            const uint8_t b1 = ((c0&maskLo2) << 4) | ( (c1&maskHi4) >> 4);   assert(b1<64); output[1] = Table[b1];
            const uint8_t b2 = (c1&maskLo4) << 2   | ( (c2&maskHi2) >> 6);   assert(b2<64); output[2] = Table[b2];
            const uint8_t b3 = (c2&maskLo6);                                 assert(b3<64); output[3] = Table[b3];

            return 4;
        }


        size_t Encode:: LengthFor(const size_t inputSize, const bool pad) noexcept
        {
            if(inputSize<=0)
            {
                return 0;
            }
            else
            {
                static const Div<size_t> api;
                Div<size_t>::Type        dv = api.call(inputSize,3);
                size_t                   res = 0;
                switch(dv.rem)
                {
                    case 1: res = (pad?4:2); break;
                    case 2: res = (pad?4:3); break;
                    default: assert(0==dv.rem); break;
                }
                res += 4 * dv.quot;
                return res;
            }

        }

        size_t Encode:: To(char * output, const void *buffer, const size_t length, const bool pad) noexcept
        {
            assert( Good(buffer,length) );
            size_t         res = 0;
            size_t         todo = length;
            const uint8_t *code = static_cast<const uint8_t *>(buffer);

            while(todo>=3)
            {
                res    += _3(output, code[0], code[1], code[2]); //4
                code   += 3;
                output += 4;
                todo   -= 3;
            }

            assert(todo<3);
            switch(todo)
            {
                case 1: res += _1(output,code[0],pad);         break;
                case 2: res += _2(output,code[0],code[1],pad); break;
                default: assert(0==todo); break;
            }

            return res;
        }

    }

}

