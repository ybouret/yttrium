#include "y/chemical/reactive/equilibria/batches.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Batches:: ~Batches() noexcept
        {
        }

        Batches:: Batches(const Equilibria &eqs) : Batch::List()
        {
            gatherSharing(eqs);
            for(Batch *batch=head;batch;batch=batch->next)
                batch->finalize();
        }

        void Batches:: gatherSharing(const Equilibria &eqs)
        {
            for(Equilibria::ConstIterator it=eqs->begin(); it!=eqs->end(); ++it)
            {
                const Equilibrium &eq = **it;
                for(Batch *batch=head;batch;batch=batch->next)
                {
                    if(batch->sharesSpeciesWith(eq))
                    {
                        (*batch) << eq;
                        lookForFusion();
                        goto CONTINUE;
                    }
                }

                pushTail( new Batch(eq) );
            CONTINUE: continue;
            }
        }


        void Batches:: lookForFusion() noexcept
        {
            Batch::List store;
            while(size>0)
            {
                AutoPtr<Batch> rhs = popHead();
                for(Batch *lhs=store.head;lhs;lhs=lhs->next)
                {
                    if(lhs->sharesSpeciesWith(*rhs))
                    {
                        lhs->mergeTail(*rhs);
                        goto CONTINUE;
                    }
                }
                store.pushTail( rhs.yield() );
            CONTINUE: continue;
            }
            swapWith(store);
        }

    }

}

