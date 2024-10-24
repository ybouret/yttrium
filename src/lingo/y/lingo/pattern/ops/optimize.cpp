#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"
#include "y/lingo/pattern/char-db.hpp"
namespace Yttrium
{
    namespace Lingo
    {

        //----------------------------------------------------------------------
        //
        //
        // return from Or/And
        //
        //
        //----------------------------------------------------------------------
        static Pattern *OptReturn(AutoPtr<Logic> &   motif,
                                  const char * const which)
        {
            assert(motif.isValid());
            assert(0!=which);

            // return with single case extraction
            switch(motif->size)
            {
                case 0: throw Specific::Exception(which,"empty content!");
                case 1: return motif->popTail();
                default:
                    break;
            }
            return motif.yield();
        }

        static inline
        Pattern * FusionBasic(const Patterns &basic)
        {
            std::cerr << "#consecutive basic=" << basic.size << std::endl;
            CharDB db;
            for(const Pattern *p=basic.head;p;p=p->next)
            {
                assert(0!=p);
                assert(p->isBasic());
                p->query(db);
            }
            std::cerr << "#db=" << db.size() << std::endl;
            Pattern * const ans = db.compile(); assert( 0 != ans );
            return ans;
        }

        //----------------------------------------------------------------------
        //
        //
        // Optimize Or
        //
        //
        //----------------------------------------------------------------------
        static inline Pattern * OptimOR(Or * const p)
        {
            assert(0!=p);
            AutoPtr<Logic> motif = p; assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // propagate optimize
            //
            //------------------------------------------------------------------
            motif->optimizing(); assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // remove duplicate
            //
            //------------------------------------------------------------------
            motif->noMultiple(); assert(motif.isValid());


            //------------------------------------------------------------------
            //
            // merge OR
            //
            //------------------------------------------------------------------
            {
                Patterns store;
                while(motif->size>0)
                {
                    assert(0!=motif->head);
                    AutoPtr<Pattern> q = motif->popHead(); assert(q.isValid());
                    if( Or::UUID == q->uuid )
                    {
                        store.mergeTail( *(q->as<Or>()) );
                        continue; // will drop Or
                    }
                    store.pushTail(q.yield());
                }
                motif->swapWith(store);
            }
            assert(motif.isValid());



            //------------------------------------------------------------------
            //
            // fusion of consecutive basic...
            //
            //------------------------------------------------------------------
            {
                Patterns store;

                while(motif->size>0)
                {
                    assert(0!=motif->head);
                    Pattern * const first = motif->popHead(); assert(0!=first);
                    if(first->isBasic())
                    {
                        Patterns basic;
                        basic.pushTail(first);
                        while(motif->size>0 && motif->head->isBasic() )
                            basic.pushTail(motif->popHead());
                        std::cerr << "#basic=" << basic.size << std::endl;
                        store.pushTail( FusionBasic(basic) );
                        //store.mergeTail(basic);
                    }
                    else
                    {
                        store.pushTail(first);
                    }
                }

                motif->swapWith(store);
            }
            assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // return
            //
            //------------------------------------------------------------------
            return OptReturn(motif,"Logic::Or");
        }


        static inline Pattern * OptimAND(And * const p)
        {
            assert(0!=p);
            AutoPtr<Logic> motif = p; assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // propagate optimize
            //
            //------------------------------------------------------------------
            motif->optimizing(); assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // merge And
            //
            //------------------------------------------------------------------
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

            //------------------------------------------------------------------
            //
            // return
            //
            //------------------------------------------------------------------
            return OptReturn(motif,"Logic::And");
        }




        Pattern * Pattern:: Optimize(Pattern * const p)
        {
            assert(0!=p);
            AutoPtr<Pattern> motif = p; assert(motif.isValid());
            std::cerr << "Optimize " << motif->callSign() << std::endl;

            switch(motif->uuid)
            {
                    // joker: internal transformation
                case Optional::UUID: motif->as<Optional>()->optimizing(); break;
                case MoreThan::UUID: motif->as<MoreThan>()->optimizing(); break;
                case Counting::UUID: motif->as<Counting>()->optimizing(); break;

                    // logic: global transformation
                case Or::  UUID: return OptimOR(  motif.yield()->as<Or>()  );
                case And:: UUID: return OptimAND( motif.yield()->as<And>() );
                default:
                    break;
            }

            assert(motif.isValid());
            return motif.yield();
        }
    }

}


