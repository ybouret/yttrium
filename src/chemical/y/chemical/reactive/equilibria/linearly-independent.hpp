//! \file

#ifndef Y_Chemical_Eqs_Deps_Included
#define Y_Chemical_Eqs_Deps_Included 1

#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/data/small/light/list/coop.hpp"
#include "y/orthogonal/family.hpp"
#include "y/associative/hash/set.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::CoopLightList<const Equilibrium> ERepo; //!< alias
        typedef ERepo::ProxyType                        EBank; //!< alias

    
        
        //______________________________________________________________________
        //
        //
        //
        //! Findind out Linearly Indepedent Equilibria
        //
        //
        //______________________________________________________________________
        class LinearlyIndependent : public Quantized, public Counted, public Proxy<const PList>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<size_t,LinearlyIndependent> Ptr; //!< alias
            typedef HashSet<size_t,Ptr>                Set; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! create and reserve resources for
            /**
             \param primary number of primary equilibria
             \param species number of active species in cluster
             \param probank persistent shared cache of PNode
             */
            explicit LinearlyIndependent(const size_t primary,
                                         const size_t species,
                                         const PBank &probank);

            //! cleanup
            virtual ~LinearlyIndependent() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const size_t & key() const noexcept;               //!< number of species
            void           init()      noexcept;               //!< initialize all
            bool keep(Prospect &pro, const Matrix<int> &topo); //!< check if eq is linearly independent of previous
            void ensure(const size_t primary);                 //!< ensure resources

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LinearlyIndependent);
            virtual ConstInterface & surrogate() const noexcept;
            PList              list;
            Orthogonal::Family qfam;
        };

        //______________________________________________________________________
        //
        //
        //
        //! Set for different number of (SubLevel) active species
        //
        //
        //______________________________________________________________________
        class LinearlyIndependentSet : public LinearlyIndependent::Set
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< uid

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit LinearlyIndependentSet();          //!< setup empty
            virtual ~LinearlyIndependentSet() noexcept; //!< cleanup

            //! update/create for primary equilibria and their species
            void operator()(const size_t primary,
                            const size_t species,
                            const PBank &probank);

            //! get pre-allocated
            LinearlyIndependent & operator[](const size_t species);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LinearlyIndependentSet);
        };

    }

}


#endif
