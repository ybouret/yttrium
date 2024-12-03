
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Cluster_sIncluded 1


#include "y/chemical/reactive/cluster.hpp"
#include "y/chemical/reactive/equilibria.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            explicit Clusters(const Equilibria &eqs, XMLog &xml);
            virtual ~Clusters() noexcept;

        private:
            Cluster::List cls;

            virtual ConstInterface & surrogate() const noexcept;

        };

    }

}

#endif

