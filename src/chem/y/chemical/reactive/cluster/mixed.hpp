//! \file

#ifndef Y_Chemical_Mixed_Equilibrium_Included
#define Y_Chemical_Mixed_Equilibrium_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class MixedEquilibrium : public Equilibrium
        {
        public:
            static String MakeName(const EList               &eqs,
                                   const WOVEn::IntegerArray &cof);

            explicit MixedEquilibrium(const String &              _name,
                                      const size_t                _indx,
                                      const EList   &             eqs,
                                      const Readable<int>       & cof,
                                      const SList   &             spc,
                                      const Readable<int> &       sto,
                                      XWritable                 & shK);

            virtual ~MixedEquilibrium() noexcept;

            XMul       xmul;
            XWritable &topK;

            const EList                     primary;
            const Small::BareHeavyList<int> weights;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xReal getK(xReal) const;
        };

    }
}

#endif

