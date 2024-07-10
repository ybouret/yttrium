
//! \file

#ifndef Y_Chemical_Equilibrium_Mixed_Included
#define Y_Chemical_Equilibrium_Mixed_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::BareHeavyList<int> Mixing;


        class MixedEquilibrium : public Equilibrium
        {
        public:
            explicit MixedEquilibrium(const size_t        topLevel,
                                      const EList         &eqs,
                                      const Readable<int> &cof);


            virtual ~MixedEquilibrium() noexcept;

            XMul         xmul;   //!< compute mixed constant
            const EList  source; //!< source equilibria
            const Mixing mixing; //!< source coefficients

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xreal_t getK(const xreal_t);
        };

    }

}

#endif
