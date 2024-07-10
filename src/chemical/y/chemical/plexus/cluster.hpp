//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/plexus/cluster/conservation/laws.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Cluster of coupled equilibria
        //
        //
        //______________________________________________________________________
        class Cluster : public EList, public Party
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
            const Cluster & operator*() const noexcept { return *this; }
            
            bool accept(const Equilibrium &) const noexcept; //!< accept equilibrium
            bool accept(const Cluster     &) const noexcept; //!< accept other cluster
            void compile(XMLog &xml);                        //!< compile all

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SList            species; //!< species in this cluster
            const Matrix<int>      Nu;      //!< primary topology
            const Matrix<unsigned> Qm;      //!< conservation matrix
            const AutoPtr<CLaws>   laws;    //!< matching laws

            Cluster *   next; //!< for list
            Cluster *   prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            void buildConservations(XMLog &);
        };
    }

}


#endif

