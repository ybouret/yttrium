

#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
#include "y/stream/io/chars.hpp"

namespace Yttrium
{

    namespace Apex
    {

        void Natural:: appendHex(IO::Chars &cache) const
        {
            const Prototype &self = CONST_PROTO(*this);
            if(self.nbits<=0)
            {
                cache << '0';
            }
            else
            {
                bool   first = true;
                size_t i     = self.bytes;
                while(i>0)
                {
                    const uint8_t b = self.getByte(--i);
                    const uint8_t l = b>>4;
                    if(first)
                    {
                        assert(b>0);
                        if(l>0) cache << Hexadecimal::Upper[l];
                        first = false;
                    }
                    else
                    {
                        cache << Hexadecimal::Upper[l];
                    }
                    cache << Hexadecimal::Upper[b&0xf];
                }
            }
        }



        void Natural:: appendDec(IO::Chars &cache) const
        {
            const Prototype &self = CONST_PROTO(*this);
            if(self.nbits<=0)
            {
                cache << '0';
            }
            else
            {
                IO::Chars     sto;
                const Natural ten(10);
                Natural       self = *this;
                Natural       q,r;
                do
                {
                    Div(q,r,self,ten); assert(r<10); assert(r.u64()<10); // will use r
                    sto << Hexadecimal::Lower[r.u64()];                  // use existing chars
                    self.xch(q);                                         // update
                } while(self.bits());
                sto.reverse();
                cache.mergeTail(sto);
            }
        }


        
        std::ostream & operator<<(std::ostream &os, const Natural &n)
        {
            //------------------------------------------------------------------
            // init cache
            //------------------------------------------------------------------
            IO::Chars cache;

            //------------------------------------------------------------------
            // fill cache
            //------------------------------------------------------------------
            if( os.flags() & std::ios_base::hex )
            {
                n.appendHex(cache);
            }
            else
            {
                n.appendDec(cache);
            }

            //------------------------------------------------------------------
            // Display
            //------------------------------------------------------------------
            Number::Display(os,cache);
            return os;


        }

    }

}


