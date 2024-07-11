//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/plexus/cluster/conservation/laws.hpp"
#include "y/chemical/plexus/fragment.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Roaming
        {
        public:
            Roaming() noexcept;
            ~Roaming() noexcept;

            const EList reacOnly;
            const EList prodOnly;
            const EList standard;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Roaming);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Cluster of coupled equilibria
        //
        //
        //______________________________________________________________________
        class Cluster : public Quantized, public EList, public Party
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Cluster> List;  //!< alias
            typedef Conservation::Laws CLaws; //!< alias
            typedef Conservation::Law  CLaw;  //!< alias
            typedef CxxArray<EList>    Lists; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cluster(const Equilibrium &first); //!< setup non empty
            virtual ~Cluster() noexcept;                //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Cluster & operator*()      const noexcept; //!< helper to print clusters
            bool accept(const Equilibrium &) const noexcept; //!< accept equilibrium
            bool accept(const Cluster     &) const noexcept; //!< accept other cluster
           
            //! compile all
            const Cluster  & compile(XMLog &      xml,
                                     Equilibria & eqs,
                                     XWritable  & shK);


            void show(std::ostream &)                     const; //!< show components
            void show(std::ostream &, const XReadable &K) const; //!< show components with constant
            void viz(OutputStream &fp, const size_t cid)  const; //!< output into cluster_id


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SList            species;   //!< species in this cluster
            const Matrix<int>      Nu;        //!< primary topology
            const Matrix<unsigned> Qm;        //!< conservation matrix
            const AutoPtr<CLaws>   laws;      //!< matching laws
            const Fragment<SList>  conserved; //!< conserved species
            const Fragment<SList>  unbounded; //!< unbounded species
            const EList            limited;   //!< limited equilibria
            const Roaming          roaming;   //!< roaming equilibria
            const Lists            order;     //!< eqs per order

            Cluster *   next; //!< for list
            Cluster *   prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            void buildConservations(XMLog &);
            void buildCombinatorics(XMLog &, Equilibria &eqs, XWritable &shK);
            bool hasConserved(const Actors &) const noexcept;

        };
    }

}


#endif

