
//! \file

#ifndef Y_Chemical_ClusterTopology_Included
#define Y_Chemical_ClusterTopology_Included 1

#include "y/chemical/plexus/cluster/content.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Create topology from content
        //
        //
        //______________________________________________________________________
        class ClusterTopology : public ClusterContent::Pointer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterTopology(XMLog &, const ClusterContent::Pointer &); //!< extract information
            virtual ~ClusterTopology() noexcept;                                //!< cleanup

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            const size_t  N;           //!< primary |equilibria|
            const size_t  M;           //!< |species|
            const iMatrix topology;    //!< N x M
            const iMatrix topologyT;   //!< transpose(topology)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterTopology);
        };
    }

}

#endif

