

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
            CONST_PROTO(*this).printHex(os);
            return os;
        }

        std::ostream & Natural:: outputDec(std::ostream &os) const
        {
            if(bits()<=0)
                os << '0';
            else
            {
                IO::Cache     cache;
                {
                    const Natural ten(10);
                    Natural       self = *this;
                    Natural       q,r;
                    do
                    {
                        Div(q,r,self,ten); assert(r<10); assert(r.u64()<10);
                        cache << Hexadecimal::Lower[r.u64()];
                        self.xch(q);
                    } while(self.bits());
                }
                Memory::BufferOf<char,Memory::Pooled> buffer(cache.size+1);
                char *str = static_cast<char *>( buffer.rw_addr() );
                {
                    size_t i = 0;
                    while(cache.size) str[i++] = cache.pullTail();
                }
                os << str;
            }

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


