
#include "y/chemical/reactive/equilibrium.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const String * Equilibrium:: Compile:: Name(const XNode * const eNode)
        {
            assert(0!=eNode);
            assert( eNode->defines<Equilibrium>() );
            const XList &       xlist = eNode->branch(); assert(xlist.size==4);
            const XNode * const label = xlist.head;      assert(label->is(Equilibrium::Label));
            const String        xname = label->lexeme().toString(1,0);
            return new String(xname);
        }


        void Equilibrium:: Compile:: Fill(Components &eq, Library &lib, const XNode * const eNode)
        {
            assert(0!=eNode);
            assert( eNode->defines<Equilibrium>() );
            const XList & xlist = eNode->branch(); assert(xlist.size==4);
            const XNode * node  = xlist.head; assert( node->is(Equilibrium::Label) );
            node=node->next;                  assert( node->is(Equilibrium::Reac)  );
            node=node->next;                  assert( node->is(Equilibrium::Prod)  );
        }
    }
}

