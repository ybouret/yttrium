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
        class Plexus : public Proxy<const Clusters>, public GraphViz::Vizible
        {
        public:
            static const char * const CallSign; //!< "Plexus"

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

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Readable<xreal> &K() const noexcept; //!< return precomputed K
            const Readable<xreal> &K(double);          //!< update K

            //! produce GrapViz code for a given order
            void graphViz(OutputStream &fp, const size_t order=1) const;


            //! render all orders
            void graphViz() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Plexus);
            virtual ConstInterface & surrogate() const noexcept;
            void buildClusters(Equilibria & all, XMLog & xml);
            void updateK(const double);

            Clusters      clusters; //!< independent clusters
            Vector<xreal> shared_K; //!< top-level constants
            
        public:
            const size_t  primaryN; //!< primary equilibria
            const size_t  maxOrder; //!< clusters' max order, start at 1, even if empty
            const size_t  maxTopId; //!< maximum top-level species index
            const size_t  maxSubId; //!< maximum sub-level species index = max cluster->lib->size = active species
            const size_t  maxEqzSz; //!< maximum number of definite equilibria in a cluster
            const SpRepo  kSpecies; //!< all kept species in definite equilibria
        };
    }

}

#endif


