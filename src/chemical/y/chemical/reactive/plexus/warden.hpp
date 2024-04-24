
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
                    Broken(const Law &, const xreal_t) noexcept;
                    Broken(const Broken&) noexcept;
                    ~Broken() noexcept;

                    friend std::ostream & operator<<(std::ostream &os, const Broken &);

                    const Law    &law;
                    const xreal_t bad;

                private:
                    Y_DISABLE_ASSIGN(Broken);
                };

                typedef Small::CoopHeavyList<const Broken> Jail;
                typedef Jail::ProxyType                    Repo;

                explicit Warden(const Clusters &clusters);
                virtual ~Warden() noexcept;
                
                void operator()(Writable<xreal_t>  &C,
                                Writable<xreal_t>  &I,
                                const GList        &G);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
                Matrix<xreal_t> dC;
                Repo            repo;
                Jail            jail;
                XAdd            xadd;

                void process(Writable<xreal_t>  &C,
                             Writable<xreal_t>  &I,
                             const Group        &G);
            };
        }

    }

}

#endif
