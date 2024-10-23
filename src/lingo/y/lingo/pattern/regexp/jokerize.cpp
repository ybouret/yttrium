#include "y/lingo/pattern/regexp/compiler.hpp"
#include "y/lingo/pattern/joker/all.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        void RXC:: Jokerize(Patterns &p, const char joker)
        {
            assert(p.size>0);
            Pattern * const motif = p.popTail();
            switch(joker)
            {
                case '*': p.pushTail( Repeated::Create(motif,0)  ); break;
                case '+': p.pushTail( Repeated::Create(motif,1)  ); break;
                case '?': p.pushTail( Optional::Create(motif)    ); break;
                case '&': p.pushTail( Pattern::IgnoreCase(motif) ); break;
                default:
                    delete motif;
                    throw Specific::Exception(CallSign, "unexpected joker '%c'",joker);
            }
        }
    }

}

