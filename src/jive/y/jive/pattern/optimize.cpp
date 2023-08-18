
#include "y/jive/pattern/all.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //----------------------------------------------------------------------
        //
        //
        // Basic
        //
        //
        //----------------------------------------------------------------------
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

        //----------------------------------------------------------------------
        //
        //
        // Logic
        //
        //
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        //
        // propagate optimize to inner patterns
        //
        //----------------------------------------------------------------------
        template <typename T>
        static inline T *OptimizeCompound(T *p)
        {
            assert(0!=p);
            AutoPtr<T> guard(p);
            {
                Patterns   target;
                Patterns  &source = p->patterns;
                while(source.size)
                    target.pushTail( Pattern::Optimize(source.popHead()) );
                source.swapWith(target);
            }
            return guard.yield();
        }

        //----------------------------------------------------------------------
        //
        // optimize And
        //
        //----------------------------------------------------------------------
        static inline Pattern * OptimizeAnd(And *p)
        {
            AutoPtr<And> motif = OptimizeCompound(p);
            if(1==motif->patterns.size)
            {
                return motif->patterns.popTail();
            }
            return motif.yield();
        }

        //----------------------------------------------------------------------
        //
        // optimize Or
        //
        //----------------------------------------------------------------------
        static inline Pattern * OptimizeOr(Or *p)
        {
            AutoPtr<Or> motif    = OptimizeCompound(p);
            Patterns   &patterns = motif->patterns;
            switch(patterns.size)
            {
                case 0: return motif.yield();
                case 1: return patterns.popTail();
                default:
                    break;
            }

            std::cerr << "[[ Optimizing Multiple Or'd]]" << std::endl;

            Patterns all;
            while(patterns.size>0)
            {
                Pattern *curr = patterns.popHead();
                assert(0!=curr);
                if(curr->isBasic())
                {
                    Patterns blk;
                    blk.pushTail(curr);
                    while(patterns.size>0 && patterns.head->isBasic())
                    {
                        blk.pushTail(patterns.popHead());
                    }
                    std::cerr << "Processing #blk=" << blk.size << std::endl;
                    all.mergeTail(blk);
                }
                else
                {
                    all.pushTail(curr);
                    while(patterns.size>0 && !patterns.head->isBasic())
                    {
                        all.pushTail(patterns.popHead());
                    }
                }
            }
            all.swapWith(patterns);

            return motif.yield();
        }


        static inline Pattern * OptimizeNone(None *p)
        {
            AutoPtr<None> motif = OptimizeCompound(p);

            return motif.yield();
        }


        //----------------------------------------------------------------------
        //
        //
        // Joker
        //
        //
        //----------------------------------------------------------------------

        template <typename T>
        T *OptimizeGuest(T *p)
        {
            assert(0!=p);
            AutoPtr<T> guard(p);
            p->optimize();
            return guard.yield();
        }

        Pattern *Pattern:: Optimize(Pattern *p)
        {
            assert(0!=p);
            switch(p->uuid)
            {
                    // basic
                case Range::UUID: return OptimizeRange(p->as<Range>());

                    // logic
                case And::  UUID: return OptimizeAnd(  p->as<And>()  );
                case Or::   UUID: return OptimizeOr(   p->as<Or>()   );
                case None:: UUID: return OptimizeNone( p->as<None>() );

                    // joker
                case Optional::  UUID: return OptimizeGuest( p->as<Optional>()  );
                case Repeating:: UUID: return OptimizeGuest( p->as<Repeating>() );
                case Counting::  UUID: return OptimizeGuest( p->as<Counting>()  );

                default:
                    break;
            }

            // left untouched
            return p;
        }

    }

}

