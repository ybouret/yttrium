//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/chem/plexus/conservation/canon.hpp"
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

        class Tier
        {
        public:
            explicit Tier() noexcept;
            virtual ~Tier() noexcept;
            const SpRepo regular;
            const SpRepo roaming;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tier);
        };

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
            void compile(Equilibria            & Eqs,
                         const Readable<xreal> & Ks,
                         XMLog                 & xml);               //!< post build

            //! update primary then replica
            void updateK(Writable<xreal> &K, const double t);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const EqRepo                  eqs;  //!< list of primary equilibria
            const SpRepo                  lib;  //!< list of active species
            const EqNode * const          last; //!< first NOT PRIMARY equilibrium
            const AutoPtr<const EqArray>  edb;  //!< equilibria database
            const AutoPtr<const SpArray>  sdb;  //!< species database
            const CxxListOf<Conservation> cll;  //!< conservation law list
            const CxxListOf<Canon>        law;  //!< list of parallel canons
            const Tier                    tier; //!< species tier

            const Matrix<int>             Nu;    //!< main topology
            const Matrix<unsigned>        Qm;    //!< conservation matrix
            Cluster *                     next;  //!< for list
            Cluster *                     prev;  //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            void createCons(XMLog &);               //!< create all conservation laws
            void makeCanons(XMLog &);               //!< create canons of conserved laws
            void connectAll(XMLog &, Equilibria &, const Readable<xreal> &); //!<
        };

        typedef CxxListOf<Cluster> Clusters; //!< alias
    }
}

#endif
