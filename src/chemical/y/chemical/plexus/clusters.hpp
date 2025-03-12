

//! \file


#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            explicit Clusters(Equilibria &eqs);
            virtual ~Clusters() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
        };

    }

}

#endif

