

#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
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
            CONST_PROTO(*this).printHex(os);
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


