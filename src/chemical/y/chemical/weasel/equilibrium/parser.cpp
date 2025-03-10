
#include "y/chemical/weasel/equilibrium/parser.hpp"
#include "y/chemical/type/xcode.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const String * Equilibrium:: Parser:: Name(const XCode &xcode)
        {
            assert( xcode->is(Equilibrium::CallSign) );
            const XList &       xlist = xcode->branch(); assert(xlist.size>0);
            const XNode * const label = xlist.head;      assert(label->is(Equilibrium::Label));
            const String        xname = label->lexeme().toString(1,0);
            return new String(xname);
        }
    }
}

