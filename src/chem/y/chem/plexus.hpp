//! \file

#ifndef Y_Chemical_Plexus_Included
#define Y_Chemical_Plexus_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/chem/plexus/cluster.hpp"
#include "y/sequence/vector.hpp"

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
            explicit Plexus(Equilibria    & Eqs,
                            XMLog         & xml); //!< setup
            virtual ~Plexus() noexcept;           //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            virtual ConstInterface & surrogate() const noexcept;
            void buildClusters(Equilibria & all,
                               XMLog &      xml);
            Clusters      clusters;
            Vector<xreal> shared_K;
        };
    }

}

#endif


