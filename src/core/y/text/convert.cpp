
#include "y/text/convert.hpp"
#include "y/system/exception.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/text/ops.hpp"
#include "y/string.hpp"

namespace Yttrium
{


    uint64_t Convert:: ToU64(const char *msg, size_t len, const char *ctx)
    {
        assert(Good(msg,len));
        static const char fn[] = "Convert";

        if(len<=0) throw Specific::Exception(fn,"no data for %s", (ctx?ctx:Core::Unknown) );
        if(len>2 && '0' == msg[0] && ('x'==msg[1] || 'X'==msg[1] ))
        {
            //------------------------------------------------------------------
            //
            // hexa
            //
            //------------------------------------------------------------------
            msg += 2;
            len -= 2;
            if(len>16) throw Specific::Exception(fn,"hexadecimal overflow for %s", (ctx?ctx:Core::Unknown) );
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
            //------------------------------------------------------------------
            //
            // decimal
            //
            //------------------------------------------------------------------
            static const uint64_t Umax = IntegerFor<uint64_t>::Maximum;
            static const uint64_t Utop = Umax / 10;
#define Y_CHECK_ADD(DELTA)                    \
if(res>(Umax-(DELTA))) goto DECIMAL_OVERFLOW; \
res += DELTA; break

            uint64_t res = 0;
            while(len-- > 0)
            {
                if(res>Utop) goto DECIMAL_OVERFLOW;

                const uint8_t c = *(msg++);
                res *= 10;
                switch(c)
                {
                    case '0': break;
                    case '1': Y_CHECK_ADD(1);
                    case '2': Y_CHECK_ADD(2);
                    case '3': Y_CHECK_ADD(3);
                    case '4': Y_CHECK_ADD(4);
                    case '5': Y_CHECK_ADD(5);
                    case '6': Y_CHECK_ADD(6);
                    case '7': Y_CHECK_ADD(7);
                    case '8': Y_CHECK_ADD(8);
                    case '9': Y_CHECK_ADD(9);
                    default:
                        throw Specific::Exception(fn,"invalid decimal '%s' for %s", ASCII::Printable::Char[c], (ctx?ctx:Core::Unknown));
                }
            }
            return res;

        DECIMAL_OVERFLOW:
            throw Specific::Exception(fn,"decimal overflow for %s", (ctx?ctx:Core::Unknown));
        }

    }

    uint64_t Convert:: ToU64(const char *msg, const char *ctx)
    {
        return ToU64(msg,StringLength(msg),ctx);
    }

    uint64_t Convert:: ToU64(const String &msg,const char *ctx)
    {
        return ToU64(msg.c_str(),msg.size(),ctx);
    }

}
