//! \file

#ifndef Y_Chemical_Cluster_Included
#define Y_Chemical_Cluster_Included 1

#include "y/chem/reactive/equilibria.hpp"
#include "y/chem/species/tier.hpp"
#include "y/chem/reactive/army.hpp"

#include "y/chem/plexus/conservation/canon.hpp"
#include "y/chem/reactive/group.hpp"


#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"

#include "y/stream/xmlog.hpp"


namespace Yttrium
{
    namespace Chemical
    {



        //______________________________________________________________________
        //
        //
        //! CxxArray to be allocated, based on Object
        //
        //______________________________________________________________________
        template <typename T>
        class ArrayOf : public Object, public CxxArray<T>
        {
        public:
            //! aliases
            Y_ARGS_DECL(T,Type);

            //! setup with default value
            inline explicit ArrayOf(const size_t n) : Object(), CxxArray<T>(n) {}

            //! setup with given value
            inline explicit ArrayOf(const size_t n, ParamType x) : Object(), CxxArray<T>(n,x) {}

            //! cleanup
            inline virtual ~ArrayOf() noexcept {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ArrayOf);
        };
        
       
        //______________________________________________________________________
        //
        //
        // Arrays for direct acces
        //
        //______________________________________________________________________
        typedef ArrayOf<Equilibrium * const>           EqArray;  //!< alias
        typedef ArrayOf<Species     * const>           SpArray;  //!< alias
        typedef ArrayOf<const EqRepo>                  EqBatch;  //!< alias
        typedef ArrayOf<const bool>                    Booleans; //!< alias

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
            static const char * const CallSign; //!< "Cluster"

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


            //! viz equilibria and species at a give order in 0..meg->size()
            void viz(OutputStream &fp, const size_t order) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const EqRepo                  all;  //!< list of ALL equilibria
            const SpRepo                  lib;  //!< list of active species
            const EqNode * const          last; //!< first NOT PRIMARY equilibrium in all
            const AutoPtr<const EqArray>  edb;  //!< PRIMARY equilibria database
            const AutoPtr<const SpArray>  sdb;  //!< species database

            const CxxListOf<Conservation> cll;  //!< conservation law list
            const CxxListOf<Canon>        law;  //!< list of parallel canons
            const AutoPtr<const Tier>     tier; //!< species tier
            const AutoPtr<const Booleans> kept; //!< conserved flag
            const AutoPtr<const Army>     army; //!< equilibria army
            const AutoPtr<const EqGroups> sGrp; //!< solving groups
            const AutoPtr<const EqGroups> eGrp; //!< equalizing groups

            const Matrix<int>             Nu;    //!< main topology
            const Matrix<unsigned>        Qm;    //!< conservation matrix
            const AutoPtr<const EqBatch>  meb;   //!< Mixed Equilibria Batchs
            Cluster *                     next;  //!< for list
            Cluster *                     prev;  //!< for list
            const Entities                eqfm;  //!< format data from all
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cluster);
            void createCons(XMLog &);               //!< create all conservation laws
            void makeCanons(XMLog &);               //!< create canons of conserved laws
            void connectAll(XMLog &, Equilibria &, const Readable<xreal> &); //!< connect all
            void createArmy(XMLog &);
            void makeGroups(XMLog &);
            
        };

        typedef CxxListOf<Cluster> Clusters; //!< alias
    }
}

#endif
