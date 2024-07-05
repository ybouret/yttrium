//! \file

#ifndef Y_Chemical_Equilibrium_Mixed_Included
#define Y_Chemical_Equilibrium_Mixed_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/container/cxx/array.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/chemical/type/constants.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Equilibrium  based on mixed primary equilibria
        //
        //
        //______________________________________________________________________
        class MixedEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Small::BareHeavyList<size_t> IList; //!< list of indices (SubLevel)
            typedef Small::BareHeavyList<int>    WList; //!< list of weights

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! setup
            /**
             \param id       name for the equilibrium, made from primaries
             \param topLevel TopLevel index
             \param weight   univocal integer weights, in SubLevel space
             \param eqset    SubLevel set of equilibria
             \param topK     TopLevel constants
             */
            //__________________________________________________________________
            explicit MixedEquilibrium(const String         &id,
                                      const size_t           topLevel,
                                      const Readable<int>   &weight,
                                      const EqSubSet        &eqset,
                                      const Constants       &topK);

            //! cleanup
            virtual ~MixedEquilibrium() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xreal_t getK(real_t);

            const IList      in;     //!< indices of primary eqs
            const WList      nu;     //!< coefficient per primary eq
            const Constants  &K_;    //!< shared constants
            const xreal_t    one;    //!< 1
            XMul             xmul;   //!< to compute K
        };

    }

}

#endif

