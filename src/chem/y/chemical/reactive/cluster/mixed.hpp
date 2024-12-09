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
        //______________________________________________________________________
        //
        //
        //
        //! Combination of primary equilibria
        //
        //
        //______________________________________________________________________
        class MixedEquilibrium : public Equilibrium
        {
        public:
            typedef Small::BareHeavyList<int>  Weights; //!< alias
            typedef Weights::NodeType          WNode;   //!< alias

            //! make human readable names from primary eqs and cof
            static String MakeName(const EList               &eqs,
                                   const WOVEn::IntegerArray &cof);

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from precomputed combination/stoichiometry
            explicit MixedEquilibrium(const String &              _name,
                                      const size_t                _indx,
                                      const EList   &             eqs,
                                      const Readable<int>       & cof,
                                      const SList   &             spc,
                                      const Readable<int> &       sto,
                                      XWritable                 & shK);

            //! cleanup
            virtual ~MixedEquilibrium() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const EList   primary; //!< list of primary equilibria
            const Weights weights; //!< their algebraic coefficients

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xReal getK(xReal) const;
            mutable XMul xmul;     //!< required for evaluation
            XWritable   &topK;     //!< topLevel K from equilibria
        };

    }
}

#endif

