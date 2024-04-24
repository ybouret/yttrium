
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
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
            const Readable<XReal> & K(const Real t);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            Cluster::List clusters;
            Constants     sharedK;
            virtual ConstInterface & surrogate() const noexcept;
        public:
            const size_t maxGroupSize; //!< over all clusters
        };
    }

}

#endif
