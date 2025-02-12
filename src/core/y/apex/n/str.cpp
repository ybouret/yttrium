#include "y/apex/natural.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/system/exception.hpp"

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
                const Natural _10  = 10;
                Natural       n    = *this; assert(n>0);
                while(n>0) {
                    Natural      q=0,r=0;
                    Natural::Div(q,r,n,_10);
                    res += '0' + unsigned( r.lsw() );
                    n.xch(q);
                }
                res.reverse();
            }
            return res;
        }

#if 0
        std::ostream & operator<<(std::ostream &os, const Natural &n) {
            Y_LOCK(*n);
            n.block->to(Plan1);
            os << *n.block;

            return os;
        }
#else
        std::ostream & operator<<(std::ostream &os, const Natural &n)
        {
            if( os.flags() & std::ios_base::hex )
            {
                os << n.toHex();
            }
            else
            {
                os << n.toDec();
            }


            return os;
        }
#endif

       
        Natural & Natural:: toHex(const char *const text, const size_t size)
        {
            assert(0!=text);
            assert(size>0);
            block->ldz(Plan1);
            Natural &self = *this;
            for(size_t i=0;i<size;++i)
            {
                const char c = text[i];
                const int  h = Hexadecimal::ToDecimal(text[i]);
                if(h<0) throw Specific::Exception(CallSign, "Invalid Hexadecimal char '%c'", c);
                self <<= 4;
                self += h;
            }
            return *this;
        }

        Natural & Natural:: toDec(const char *const text, const size_t size)
        {
            assert(0!=text);
            assert(size>0);
            block->ldz(Plan1);
            const Natural ten  = 10;
            Natural     & self = *this;
            for(size_t i=0;i<size;++i)
            {
                const char c = text[i];
                if( !isdigit(c) ) throw Specific::Exception(CallSign,"Invalad Decimal char '%c'",c);
                self *= 10;
                self += c-'0';
            }

            return *this;
        }


    }
}
