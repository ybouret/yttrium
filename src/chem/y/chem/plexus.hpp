//! \file

#ifndef Y_Chemical_Plexus_Included
#define Y_Chemical_Plexus_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/chem/plexus/cluster.hpp"

namespace Yttrium
{

    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Management of equilibria to partition solvers
        //
        //
        //______________________________________________________________________
        class Plexus : public Proxy<const Clusters>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Plexus(Equilibria &, XMLog &); //!< setup
            virtual ~Plexus() noexcept;             //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            virtual ConstInterface & surrogate() const noexcept;
            void buildClusters(Equilibria &, XMLog &xml);
            Clusters clusters;
        };
    }

}

#endif


