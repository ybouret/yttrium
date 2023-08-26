
#include "y/text/ascii/convert.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/text/ops.hpp"
#include "y/type/ints.hpp"
#include "y/string.hpp"

namespace Yttrium
{

    namespace ASCII
    {
        int64_t Convert::  ToI64(const char *msg, size_t len, const char *ctx)
        {
            assert(Good(msg,len));
            static const char fn[] = "Convert to int64_t";

            bool neg = false;

            if(len<=0) throw Specific::Exception(fn,"no data for %s", (ctx?ctx:Core::Unknown) );

            if('-'==msg[0])
            {
                neg = true;
                --len;
                ++msg;
                if(len<=0) throw Specific::Exception(fn,"no data for negative %s", (ctx?ctx:Core::Unknown) );
            }

            const uint64_t Umax = neg ? uint64_t(-IntegerFor<int64_t>::Minimum) : uint64_t(IntegerFor<int64_t>::Maximum);
            const uint64_t Utop = Umax/10;

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
            return neg ? -res:res;

        DECIMAL_OVERFLOW:
            throw Specific::Exception(fn,"decimal overflow for %s", (ctx?ctx:Core::Unknown));
        }

        int64_t Convert:: ToI64(const char *msg, const char *ctx)
        {
            return ToI64(msg,StringLength(msg),ctx);
        }

        int64_t Convert:: ToI64(const String &msg,const char *ctx)
        {
            return ToI64(msg.c_str(),msg.size(),ctx);
        }

    }

}


