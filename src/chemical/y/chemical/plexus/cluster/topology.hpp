
//! \file

#ifndef Y_Chemical_ClusterTopology_Included
#define Y_Chemical_ClusterTopology_Included 1

#include "y/chemical/plexus/cluster/content.hpp"
#include "y/stream/xmlog.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Matrix<int> iMatrix;

        class ClusterTopology : public ClusterContent::Pointer
        {
        public:
            explicit ClusterTopology(XMLog &xml, const ClusterContent::Pointer &ptr);
            virtual ~ClusterTopology() noexcept;

            const size_t  N;
            const size_t  M;
            const iMatrix topology;
            const iMatrix topologyT;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterTopology);
        };
    }

}

#endif

