//! \file


#ifndef Y_Chemical_Plexus_Included
#define Y_Chemical_Plexus_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/chem/plexus/cluster.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        class Plexus : public Proxy<const Clusters>
        {
        public:
            explicit Plexus(Equilibria &, XMLog &);
            virtual ~Plexus() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            virtual ConstInterface & surrogate() const noexcept;
            void buildClusters(Equilibria &, XMLog &xml);

            Clusters clusters;

        };
    }

}

#endif


