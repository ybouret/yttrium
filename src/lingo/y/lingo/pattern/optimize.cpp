#include "y/lingo/pattern/all.hpp"

namespace Yttrium
{
    namespace Lingo
    {


        static inline Pattern * Optim(Or * const p)
        {
            AutoPtr<Or> motif = p;

            // propagate optimmize
            motif->optimize();

            // remove duplicate
            motif->noMultiple();

            // merge OR
            {
                Patterns store;
                while(motif->size>0)
                {
                    AutoPtr<Pattern> q = motif->popHead();
                    if( Or::UUID == q->uuid )
                    {
                        store.mergeTail( *(q->as<Or>()) );
                        continue;
                    }
                    store.pushTail(q.yield());
                }
                motif->swapWith(store);
            }

            // fusion of consecutive basic...
            return motif.yield();
        }


        static inline Pattern * Optim(And * const p)
        {
            AutoPtr<And> motif = p;

            // propagate optimmize
            motif->optimize();


            // merge And
            {
                Patterns store;
                while(motif->size>0)
                {
                    AutoPtr<Pattern> q = motif->popHead();
                    if( And::UUID == q->uuid )
                    {
                        store.mergeTail( *(q->as<And>()) );
                        continue;
                    }
                    store.pushTail(q.yield());
                }
                motif->swapWith(store);
            }

            return motif.yield();
        }




        Pattern * Pattern:: Optimize(Pattern * const p)
        {
            assert(0!=p);
            AutoPtr<Pattern> motif = p;

            switch(motif->uuid)
            {
                    // joker
                case Optional::UUID: motif->as<Optional>()->optimize(); break;
                case Repeated::UUID: motif->as<Repeated>()->optimize(); break;
                case Counting::UUID: motif->as<Counting>()->optimize(); break;

                    // logic
                case Or::  UUID: return Optim( motif.yield()->as<Or>() );
                case And:: UUID: return Optim( motif.yield()->as<And>() );

                default:
                    break;
            }

            return motif.yield();
        }
    }

}


