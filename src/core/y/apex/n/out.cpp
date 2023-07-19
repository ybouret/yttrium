

#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
#include "y/io/cache.hpp"

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
                while(cache.size) os << cache.pullTail();
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


