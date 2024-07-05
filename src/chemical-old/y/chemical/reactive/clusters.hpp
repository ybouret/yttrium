
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::BareLightList<const Conservation::Group> GList; //!< list of group
        typedef GList::NodeType                                 GNode; //!< alias>

        //______________________________________________________________________
        //
        //
        //
        //! clusters of chemically linked equilibria
        //
        //
        //______________________________________________________________________
        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            //! setup from primary equilibria
            /**
             \param eqs  primary equilbria, updated
             \param topK shared constants
             \param xml  for verbose output
             */
            explicit Clusters(Equilibria      &eqs,
                              const Constants &topK,
                              XMLog           &xml) ;

            //! cleanup
            virtual ~Clusters() noexcept;


            //! cleanup
            const Readable<xreal_t> & K(const real_t t);

            //! saving full reactive systems
            void graphViz(const String &root) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            Cluster::List clusters;
            Constants     sharedK;
            virtual ConstInterface & surrogate() const noexcept;

        public:
            GList        groups; //!< all conservation groups
            const size_t maxSPC; //!< maxSpeciesPerCluster
            const size_t maxCPG; //!< maxConservationsPerGroup
            const size_t maxSPG; //!< maxSpeciesPerGroup
            const size_t maxDEG; //!< maximum degree of combinations
            const size_t maxCPC; //!< maximum Constrollers Per Cluster
        };
    }

}

#endif
