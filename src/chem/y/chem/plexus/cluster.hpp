//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/chem/plexus/conservation.hpp"
#include "y/stream/xmlog.hpp"

#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //----------------------------------------------------------------------
        //
        // List of species
        //
        //----------------------------------------------------------------------
        typedef Small::BareLightList<const Species>     SpRepo; //!< alias
        typedef SpRepo::NodeType                        SpNode; //!< alias

        //----------------------------------------------------------------------
        //
        // List of equilibria
        //
        //----------------------------------------------------------------------
        typedef Small::BareLightList<const Equilibrium> EqRepo; //!< alias
        typedef EqRepo::NodeType                        EqNode; //!< alias

        //----------------------------------------------------------------------
        //
        // Arrays for direct acces
        //
        //----------------------------------------------------------------------
        typedef CxxArray<Equilibrium * const>           EqArray; //!< alias
        typedef CxxArray<Species     * const>           SpArray; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Cluster of dependent species and equilibria
        //
        //
        //______________________________________________________________________
        class Cluster : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Cluster(const Equilibrium &first); //!< setup non empty
            virtual ~Cluster() noexcept;                //!< clenaup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool tiedTo(const Equilibrium &) const noexcept; //!< check common species
            bool tiedTo(const Cluster &)     const noexcept; //!< check common species
            void enroll(const Equilibrium &);                //!< load eq+species
            void compile(Equilibria &all, XMLog &);          //!< post build

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const EqRepo                  eqs; //!< list of eqs
            const SpRepo                  lib; //!< list of active species
            const AutoPtr<const EqArray>  edb; //!< equilibria database
            const AutoPtr<const SpArray>  sdb; //!< species database
            const CxxListOf<Conservation> cll; //!< conservation law list
            

            const Matrix<int>      Nu;    //!< topology
            const Matrix<unsigned> Qm;    //!< conservation matrix
            Cluster *              next;  //!< for list
            Cluster *              prev;  //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            void buildConservations(XMLog &);
        };

        typedef CxxListOf<Cluster> Clusters; //!< alias
    }
}

#endif
