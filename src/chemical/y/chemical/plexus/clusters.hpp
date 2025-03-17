

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

        typedef Vector<xreal_t,MemoryModel> XVector;

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
            explicit Clusters(XMLog &xml, Equilibria &eqs); //!< full setup
            virtual ~Clusters() noexcept;                   //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Clusters);
            Y_PROXY_DECL();
            Cluster::List   my;
            XVector         tlK;

        public:
            const XReadable &K;
            
        };
        
    }

}

#endif

