
//! \file

#ifndef Y_Chemical_Equilibrium_Mixed_Included
#define Y_Chemical_Equilibrium_Mixed_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::BareHeavyList<int> MixList;
        typedef MixList::NodeType         MixNode;

        class MixedEquilibrium : public Equilibrium
        {
        public:
            explicit MixedEquilibrium(const size_t        topLevel,
                                      const EList         &eqs,
                                      const Readable<int> &cof,
                                      XWritable           &shK);


            virtual ~MixedEquilibrium() noexcept;

            XMul          xmul;   //!< compute mixed constant
            XWritable &   topK;   //!< top level K
            const EList   source; //!< source equilibria
            const MixList mixing; //!< source coefficients

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xreal_t getK(const xreal_t);
        };

    }

}

#endif
