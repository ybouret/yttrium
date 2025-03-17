//! \file

#ifndef Y_Chemical_ClusterBuilder_Included
#define Y_Chemical_ClusterBuilder_Included 1

#include "y/chemical/plexus/cluster/knot.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
      
        //______________________________________________________________________
        //
        //
        //
        //! create independent knots of dependent equilibria
        //
        //
        //______________________________________________________________________
        class ClusterBuilder : public Proxy<const ListOf<ClusterKnot> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit ClusterBuilder(XMLog &xml, Equilibria &eqs); //!< make partition from TopLevel eqs
            virtual ~ClusterBuilder() noexcept;                   //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ClusterBuilder);
            Y_PROXY_DECL();
            void startClusterFor(Equilibrium &eq); //!< create with resource
            CxxListOf<ClusterKnot> cls;            //!< clusters
        };
    }

}

#endif

