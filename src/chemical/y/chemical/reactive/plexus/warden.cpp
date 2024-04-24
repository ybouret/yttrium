
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
            jail(repo)
            {
                repo->reserve(dC.rows);
                std::cerr << "dC=" << dC << std::endl;
            }
            
            Warden:: ~Warden() noexcept
            {
            }
            
            void Warden:: process(Writable<xreal_t>         &C,
                                  Writable<xreal_t>         &I,
                                  const Conservation::Group &G)
            {
                const size_t nlaw = G->size; assert(nlaw>0); assert(nlaw<=dC.rows);
                jail.free(); assert(repo->stowage()>=nlaw);
            }
            
        }


    }

}


