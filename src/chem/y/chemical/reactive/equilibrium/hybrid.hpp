
//! \file

#ifndef Y_Chemical_Hybrid_Equilibrium_Included
#define Y_Chemical_Hybrid_Equilibrium_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class HybridEquilibrium : public Equilibrium
        {
        public:

            //! make name from linear combination of primary equilibria
            /**
             \param primaryList list of primary equilibria with their SubLevel
             \param coefficient list of weights
             */
            static String MakeName(const EList &         primaryList,
                                   const Readable<int> & coefficient);

            virtual ~HybridEquilibrium() noexcept;
            explicit HybridEquilibrium(const String & _name,
                                       const size_t   _indx,
                                       const EList  &        elist,
                                       const Readable<int> & ecoef,
                                       const SList  &        slist,
                                       const Readable<int>  &scoef);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(HybridEquilibrium);
            virtual xReal getK(xReal) const;

            mutable XMul                          xmul;
            const   EList                         eqs;
            const Small::BareHeavyList<const int> cof;

        };
    }

}

#endif

