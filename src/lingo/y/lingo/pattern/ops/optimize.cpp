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
        static Pattern *OptReturn(AutoPtr<Logic> &motif, const char * const which)
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
            CharDB db;
            for(const Pattern *p=basic.head;p;p=p->next)
            {
                assert(p->isBasic());
                p->query(db);
            }
            return db.compile();
        }

        //----------------------------------------------------------------------
        //
        //
        // Optimize Or
        //
        //
        //----------------------------------------------------------------------
        static inline Pattern * Optim(Or * const p, int nesting)
        {
            assert(0!=p);
            AutoPtr<Logic> motif = p; assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // propagate optimize
            //
            //------------------------------------------------------------------
            motif->optimizing(nesting); assert(motif.isValid());

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
                    if(motif->head->isBasic())
                    {
                        Patterns basic;
                        basic.pushTail(motif->popHead());
                        while(motif->size>0 && motif->head->isBasic() )
                            basic.pushTail(motif->popHead());
                        store.pushTail( FusionBasic(basic) );
                    }
                    else
                    {
                        store.pushTail(motif->popHead());
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


        static inline Pattern * Optim(And * const p, int nesting)
        {
            assert(0!=p);
            AutoPtr<Logic> motif = p; assert(motif.isValid());

            //------------------------------------------------------------------
            //
            // propagate optimize
            //
            //------------------------------------------------------------------
            motif->optimizing(nesting); assert(motif.isValid());

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




        Pattern * Pattern:: Optimize(Pattern * const p, int nesting)
        {
            assert(0!=p);
            AutoPtr<Pattern> motif = p; assert(motif.isValid());
            Core::Indent(std::cerr,nesting) << "<enter " << motif->callSign() << ">" << std::endl;
            ++nesting;

            switch(motif->uuid)
            {
                    // joker: internal transformation
                case Optional::UUID: motif->as<Optional>()->optimizing(nesting); break;
                case MoreThan::UUID: motif->as<MoreThan>()->optimizing(nesting); break;
                case Counting::UUID: motif->as<Counting>()->optimizing(nesting); break;

                    // logic: global transformation
                case Or::  UUID: return Optim( motif.yield()->as<Or>(),  nesting);
                case And:: UUID: return Optim( motif.yield()->as<And>(), nesting);

                default:
                    break;
            }
            --nesting;

            assert(motif.isValid());
            Core::Indent(std::cerr,nesting) << "<leave " << motif->callSign() << ">" << std::endl;
            return motif.yield();
        }
    }

}


