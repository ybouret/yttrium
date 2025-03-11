
#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const String * Equilibrium:: Parser:: Name(const XCode &xcode)
        {
            assert( xcode->is(Equilibrium::CallSign) );
            const XList &       xlist = xcode->branch(); assert(xlist.size==4);
            const XNode * const label = xlist.head;      assert(label->is(Equilibrium::Label));
            const String        xname = label->lexeme().toString(1,0);
            return new String(xname);
        }


        void Equilibrium:: Parser:: Fill(Components &eq, Library &lib, const XCode &xcode)
        {
            assert( xcode->is(Equilibrium::CallSign) );
            const XList & xlist = xcode->branch(); assert(xlist.size==4);
            const XNode * node  = xlist.head; assert( node->is(Equilibrium::Label) );
            node=node->next;                  assert( node->is(Equilibrium::Reac)  );
            node=node->next;                  assert( node->is(Equilibrium::Prod)  );

        }
    }
}

