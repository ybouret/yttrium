

#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
#include "y/io/cache.hpp"
#include "y/memory/buffer/of.hpp"

namespace Yttrium
{

    namespace Apex
    {

        std::ostream & Natural:: outputHex(std::ostream &os) const
        {
            const Prototype &self = CONST_PROTO(*this);
            if(self.nbits<=0) { os << '0'; return os; }

            Memory::BufferOf<char,Memory::Pooled> buffer(2*self.bytes+1);
            bool   first = true;
            size_t i     = self.bytes;
            size_t j     = 0;
            while(i>0)
            {
                const uint8_t b = self.getByte(--i);
                const uint8_t l = b>>4;
                if(first)
                {
                    assert(b>0);
                    if(l>0) buffer[j++] = Hexadecimal::Upper[l];
                    first = false;
                }
                else
                {
                    buffer[j++] = Hexadecimal::Upper[l];
                }
                buffer[j++] = Hexadecimal::Upper[b&0xf];
            }
            os << &buffer[0];
            return os;
        }

        std::ostream & Natural:: outputDec(std::ostream &os) const
        {
            if(bits()<=0) { os << '0'; return os; }

            //------------------------------------------------------------------
            // IO cache since chars come in rever
            //------------------------------------------------------------------
            IO::Cache     cache;
            {
                const Natural ten(10);
                Natural       self = *this;
                Natural       q,r;
                do
                {
                    Div(q,r,self,ten); assert(r<10); assert(r.u64()<10); // will use r
                    cache << Hexadecimal::Lower[r.u64()];                // use existing chars
                    self.xch(q);                                         // update
                } while(self.bits());
            }
            //------------------------------------------------------------------
            // construct C-string
            //------------------------------------------------------------------
            Memory::BufferOf<char,Memory::Pooled> buffer(cache.size+1);
            {
                size_t i = 0;
                while(cache.size) buffer[i++] = cache.pullTail();
            }

            //------------------------------------------------------------------
            // done
            //------------------------------------------------------------------
            os << &buffer[0];;
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Natural &n)
        {
            if( os.flags() & std::ios_base::hex )
            {
                return n.outputHex(os);
            }
            else
            {
                return n.outputDec(os);
            }
        }

    }

}


