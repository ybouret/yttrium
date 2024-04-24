
//! \file

#ifndef Y_Chemical_Warden_Included
#define Y_Chemical_Warden_Included 1

#include "y/chemical/reactive/clusters.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            class Warden
            {
            public:
                class Broken
                {
                public:

                    Broken(const Law &, const xreal_t, Writable<xreal_t> &) noexcept;
                    Broken(const Broken&) noexcept;
                    ~Broken() noexcept;

                    
                    const Law               &law;
                    xreal_t                  bad;
                    Writable<xreal_t>       &del;

                private:
                    Y_DISABLE_ASSIGN(Broken);
                };
                typedef Small::CoopHeavyList<Broken> BrokenList;
                typedef BrokenList::ProxyType        BrokenRepo;
                typedef BrokenList::NodeType         BrokenNode;
                

                explicit Warden(const Clusters &clusters);
                virtual ~Warden() noexcept;
                
                void operator()(Writable<xreal_t>  &C,
                                Writable<xreal_t>  &I,
                                const GList        &G,
                                XMLog              &xml);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
                Matrix<xreal_t> dC;
                BrokenRepo    repo;
                BrokenList    jail;
                XAdd          xadd;

                void process(Writable<xreal_t>  &C,
                             Writable<xreal_t>  &I,
                             const Group        &G,
                             XMLog              &xml);
            };
        }

    }

}

#endif
