
#include "y/lingo/pattern/matching.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Matching:: ~Matching() noexcept
        {
        }

        Matching:: Matching(const Matching &_) :
        Token(),
        motif( _.motif->clone() )
        {
        }


        bool Matching:: exactly_(Module * const m)
        {
            assert(0!=m);
            Source source(m);
            release();
            if(motif->takes(*this,source,0))
            {
                return !source.ready();
            }
            else
            {
                return false;
            }
        }

        bool Matching:: somehow_(Module * const m)
        {
            assert(0!=m);
            Source source(m);
            release();

            while(!motif->takes(*this,source,0))
            {
                if(!source.ready()) return false;
                source.skip();
            }

            return true;
        }


    }
}
