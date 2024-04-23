
//! \file

#ifndef Y_Chemical_Cluster_Topology_Included
#define Y_Chemical_Cluster_Topology_Included 1

#include "y/chemical/reactive/equilibria/fragment.hpp"
#include "y/chemical/type/constants.hpp"
#include "y/container/matrix.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class ClusterTopology : public Fragment
        {
        public:
            explicit ClusterTopology(const Fragment    &fragment,
                                     const Constants   &topK,
                                     XMLog             &xml);

            virtual ~ClusterTopology() noexcept;

        protected:
            Constants                   sharedK; //!< TopLevel constants
        public:
            const SList                 species;   //!< SubLevel, primary species
            const Matrix<int>           Nu;        //!< SubLevel topology
            const SpSubSet              spset;     //!< table of SubLevel species
            const EqSubSet              eqset;     //!< table of SubLevel equilibria
            const EqFormatter           eqfmt;     //!< helper to display
            const Entities              spfmt;     //!< helper to display
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterTopology);
        };
    }

}

#endif

