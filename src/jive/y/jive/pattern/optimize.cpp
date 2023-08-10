
#include "y/jive/pattern/all.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        static inline
        Pattern *OptimizeRange(Range *p)
        {
            assert(0!=p);
            if(p->lower>=p->upper)
            {
                const AutoPtr<Pattern> keep(p);
                return new Single(p->lower);
            }
            else
            {
                return p;
            }
        }


        Pattern *Pattern:: Optimize(Pattern *p)
        {
            assert(0!=p);
            switch(p->uuid)
            {
                case Range::UUID: return OptimizeRange(p->as<Range>());
                default:
                    break;
            }

            // left untouched
            return p;
        }

    }

}

