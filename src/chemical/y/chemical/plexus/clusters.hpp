
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            explicit Clusters(Equilibria &, XMLog &);
            virtual ~Clusters() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            virtual ConstInterface & surrogate() const noexcept;
            Cluster::List cls;

        };

    }

}

#endif

