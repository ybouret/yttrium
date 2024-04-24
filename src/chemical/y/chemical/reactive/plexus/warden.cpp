
#include "y/chemical/reactive/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            Warden:: Warden(const Clusters &clusters) :
            dC(clusters.maxCPG,clusters.maxCPG <=0 ? 0 : clusters.maxSPC ),
            repo(),
            jail(repo),
            xadd(dC.cols)
            {
                repo->reserve(dC.rows);
                std::cerr << "dC=" << dC << std::endl;
            }

            Warden:: ~Warden() noexcept
            {
            }

            void Warden:: process(Writable<xreal_t>  &C,
                                  Writable<xreal_t>  &I,
                                  const Group        &G)
            {
                const size_t nlaw = G->size; assert(nlaw>0); assert(nlaw<=dC.rows);
                jail.free(); assert(repo->stowage()>=nlaw);

                size_t nbad=0;
                for(const LawNode *node=G->head;node;node=node->next)
                {
                    const Law    &law = **node;
                    const xreal_t xs  = law.required(dC[nbad+1],SubLevel, C, TopLevel, xadd);
                }

            }

        }


    }

}


