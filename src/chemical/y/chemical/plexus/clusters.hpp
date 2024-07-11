
//! \file

#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of independant clusters
        //
        //
        //______________________________________________________________________
        class Clusters : public Proxy<const Cluster::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Clusters(Equilibria &, XMLog &); //!< setup from equilibria
            virtual ~Clusters() noexcept;             //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const XReadable & K(const xreal_t t);            //!< in order K evaluation
            void              show(std::ostream &os)  const; //!< show components
            void              showK(std::ostream &os) const; //!< show components with constants

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            virtual ConstInterface & surrogate() const noexcept;
            Cluster::List   cls;
            Vector<xreal_t> shK;

        public:
            const size_t  maxSPC;  //!< max Species per cluster
            const size_t  maxLPG;  //!< max Laws per Group
            const SList   species; //!< all reactive species
        };

    }

}

#endif

