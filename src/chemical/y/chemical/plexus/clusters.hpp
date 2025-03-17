

//! \file


#ifndef Y_Chemical_Clusters_Included
#define Y_Chemical_Clusters_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Vector<xreal_t,MemoryModel> XVector; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Independent Clusters of dependent species
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

            //! full setup
            /**
             \param xml for verbosity
             \param eqs primary equilibria
             \param t0  time to evaluate constants
             */
            explicit Clusters(XMLog &       xml,
                              Equilibria &  eqs,
                              const xreal_t t0);

            //! cleanuo
            virtual ~Clusters() noexcept;

            const XReadable &operator()(const xreal_t);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            Y_PROXY_DECL();
            Cluster::List   my;
            XVector         tlK;

        public:
            const XReadable &K; //!< last computed constant

        };

    }

}

#endif

