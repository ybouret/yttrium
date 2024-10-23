#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        static Pattern *OptReturn(AutoPtr<Logic> &motif, const char * const which)
        {
            assert(0!=which);
            // return
            switch(motif->size)
            {
                case 0: throw Specific::Exception(which,"empty content!");
                case 1: return motif->popTail();
                default:
                    break;
            }
            return motif.yield();
        }

        static inline Pattern * Optim(Or * const p)
        {
            AutoPtr<Logic> motif = p;

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

            // return
            return OptReturn(motif,"Logic::Or");

        }


        static inline Pattern * Optim(And * const p)
        {
            AutoPtr<Logic> motif = p;

            // propagate optimize
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

            // return
            return OptReturn(motif,"Logic::And");
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


