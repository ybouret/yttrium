#include "y/apex/natural.hpp"
#include "y/text/hexadecimal.hpp"

namespace Yttrium
{
    namespace Apex
    {

        static inline
        void printFirst(String &res, const uint8_t byte)
        {
            const uint8_t hi = (byte>>4)&0xf;
            if(hi>0)
                res += Hexadecimal::Lower[hi];
            res     += Hexadecimal::Lower[byte&0xf];
        }

        static inline
        void printExtra(String &res, const uint8_t byte)
        {
            res += Hexadecimal::Text[byte];
        }

        String Natural:: toHex() const
        {
            volatile AutoLock guard(*this);
            const Jig1 &jig = block->make<Plan1>();
            String res;
            size_t n = jig.words;
            if(n<=0)
            {
                res += '0';
            }
            else
            {
                const uint8_t * const b = jig.word;
                printFirst(res,b[--n]);
                while(n-- > 0 )
                    printExtra(res,b[n]);
            }
            return res;
        }

        String Natural:: toDec() const
        {
            String res;
            if(block->bits<=0)
            {
                res = '0';
            }
            else
            {
                const Natural _10 = 10;
            }
            return res;
        }


    }
}
