
#include "y/chemical/weasel/equilibrium/parser.hpp"
#include "y/chemical/type/xcode.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const String * Equilibrium:: Parser:: Name(const XCode &xcode)
        {
            assert( xcode->is(Equilibrium::CallSign) );
            const XList &list = xcode->branch(); assert(list.size>0);
            
        }
    }
}

