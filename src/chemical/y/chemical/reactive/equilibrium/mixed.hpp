//! \file

#ifndef Y_Chemical_Equilibrium_Mixed_Included
#define Y_Chemical_Equilibrium_Mixed_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/container/cxx/array.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/mkl/antelope/mul.hpp"
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
            typedef Small::BareHeavyList<size_t> IList;
            typedef Small::BareHeavyList<int>    WList;
            const IList                  in;
            const WList                  nu;
            const Constants              &K_;
            const XReal                  one;
            MKL::Antelope::Mul<XReal>    xmul;

            explicit MixedEquilibrium(const String         &id,
                                      const size_t           topLevel,
                                      const Readable<int>   &weight,
                                      const EqSubSet        &eqset,
                                      const Constants       &topK);

            virtual ~MixedEquilibrium() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual XReal getK(Real);
        };

    }

}

#endif

