
//! \file

#ifndef Y_Chemical_Warden_Included
#define Y_Chemical_Warden_Included 1

#include "y/chemical/reactive/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            class Warden
            {
            public:
                typedef Small::CoopLightList<const Law> Jail;
                typedef Jail::ProxyType                 Repo;

                explicit Warden(const Clusters &clusters);
                virtual ~Warden() noexcept;

                void process(Writable<xreal_t>  &C,
                             Writable<xreal_t>  &I,
                             const Group        &G);

                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
                Matrix<xreal_t> dC;
                Repo            repo;
                Jail            jail;
                XAdd            xadd;
            };
        }

    }

}

#endif
