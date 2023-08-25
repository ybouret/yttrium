
#include "y/text/convert.hpp"
#include "y/system/exception.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ascii/printable.hpp"
#include <iostream>

namespace Yttrium
{


    uint64_t Convert:: ToU64(const char *msg, size_t len, const char *ctx)
    {
        assert(Good(msg,len));
        static const char fn[] = "Convert";

        if(len<=0) throw Specific::Exception(fn,"no data for %s", (ctx?ctx:Core::Unknown) );
        if(len>2 && '0' == msg[0] && ('x'==msg[1] || 'X'==msg[1] ))
        {
            // hexa
            msg += 2;
            len -= 2;
            if(len>16) throw Specific::Exception(fn,"overflow for hexadecimal %s", (ctx?ctx:Core::Unknown) );
            uint64_t res = 0;
            while(len-- > 0)
            {
                const uint8_t c = *(msg++);
                const int     h = Hexadecimal::ToDecimal(c);
                if(h<0) throw Specific::Exception(fn,"invalid hexadecimal '%s' for %s", ASCII::Printable::Char[c],(ctx?ctx:Core::Unknown));
                res <<= 4;
                res |= uint8_t(h);
            }

            return res;
        }
        else
        {
            // decimal
            std::cerr << "-> dec" << std::endl;
            uint64_t res = 0;
            while(len-- > 0)
            {
                const uint8_t c = *(msg++);
                std::cerr << "c=" << char(c) << std::endl;
                res *= 10;
                switch(c)
                {
                    case '0': break;
                    case '1': ++res; break;
                    case '2': res += 2; break;
                    case '3': res += 3; break;
                    case '4': res += 4; break;
                    case '5': res += 5; break;
                    case '6': res += 6; break;
                    case '7': res += 7; break;
                    case '8': res += 8; break;
                    case '9': res += 9; break;
                    default:
                        throw Specific::Exception(fn,"invalid decimal '%s' for %s", ASCII::Printable::Char[c],(ctx?ctx:Core::Unknown));
                }
            }
            return res;
        }

    }



}
