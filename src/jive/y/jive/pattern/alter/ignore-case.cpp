
#include "y/jive/pattern/all.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Pattern * IgnoreCaseOfBasic(Pattern *p)
        {
            static const char   lower[32] = "abcdefghijklmnopqrstuvwxyz";
            static const char   upper[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            static const size_t count     = 26;

            const AutoPtr<const Pattern> keep(p);
            FirstChars fc;
            p->query(fc);

            for(size_t i=0;i<count;++i)
            {
                const uint8_t l = lower[i];
                const uint8_t u = upper[i];
                if( fc[l] ) fc.include(u);
                if( fc[u] ) fc.include(l);
            }

            return fc.makeOr();
        }


        Pattern * IgnoreCaseOfCompound(Compound *p)
        {
            AutoPtr<Pattern> guard(p);

            {
                Patterns ign;
                while(p->count>0)
                {
                    ign.pushTail( Pattern::IgnoreCase( p->patterns.popHead()) );
                }
                p->patterns.swapWith(ign);
            }

            return guard.yield();
        }

        Pattern *IgnoreCaseOfGuest(Guest *p)
        {
            AutoPtr<Pattern> guard(p);
            p->ignoreCase();
            return guard.yield();
        }

        Pattern * Pattern::IgnoreCase(Pattern *p)
        {
            assert(0!=p);

            AutoPtr<Pattern> guard(p);

            switch(p->uuid)
            {
                case Single  ::UUID:
                case Range   ::UUID:
                case Exclude ::UUID:
                    return IgnoreCaseOfBasic(guard.yield());

                case Or::   UUID: return IgnoreCaseOfCompound( guard.yield()->as<Or>()   );
                case And::  UUID: return IgnoreCaseOfCompound( guard.yield()->as<And>()  );
                case None:: UUID: return IgnoreCaseOfCompound( guard.yield()->as<None>() );

                case Optional::  UUID: return IgnoreCaseOfGuest( guard.yield()->as<Optional>()  );
                case Repeating:: UUID: return IgnoreCaseOfGuest( guard.yield()->as<Repeating>() );
                case Counting::  UUID: return IgnoreCaseOfGuest( guard.yield()->as<Counting>()  );

                default:
                    break;
            }


            return guard.yield();
        }
    }

}
