
//! \file

#ifndef Y_Chemical_Equilibrium_Mixed_Included
#define Y_Chemical_Equilibrium_Mixed_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/data/small/heavy/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        typedef Small::BareHeavyList<int> MixList; //!< alias
        typedef MixList::NodeType         MixNode; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Combination of weighted equilibria
        //
        //
        //______________________________________________________________________
        class MixedEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             \param topLevel top-level index
             \param eqs      list of primary
             \param cof      coefficient of primary
             \param shK      shared persistent constants
             */
            explicit MixedEquilibrium(const size_t        topLevel,
                                      const EList         &eqs,
                                      const Readable<int> &cof,
                                      XWritable           &shK);

            //! cleanup
            virtual ~MixedEquilibrium() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            XMul          xmul;   //!< compute mixed constant
            XWritable &   topK;   //!< top level K
            const MixList coef;   //!< source coefficients

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MixedEquilibrium);
            virtual xreal_t getK(const xreal_t);
        };

    }

}

#endif
