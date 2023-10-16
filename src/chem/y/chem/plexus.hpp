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
        class Plexus : public Proxy<const Clusters>, public Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Plexus(Equilibria    & Eqs,
                            XMLog         & xml,
                            const double    t0); //!< setup
            virtual ~Plexus() noexcept;          //!< cleanup
            
            const Readable<xreal> &K() const noexcept;
            const Readable<xreal> &K(double);

            void graphViz(OutputStream &fp) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            virtual ConstInterface & surrogate() const noexcept;
            void buildClusters(Equilibria & all, XMLog & xml);
            void updateK(const double);

            Clusters      clusters;
            Vector<xreal> shared_K;
            const size_t  N;


        };
    }

}

#endif


