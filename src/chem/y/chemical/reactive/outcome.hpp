//! \file

#ifndef Y_Chemical_Outcome_Included
#define Y_Chemical_Outcome_Included 1

#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! outcome of an aftemath
        //
        //
        //______________________________________________________________________
        class Outcome
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from
            Outcome(const Situation  &_st,
                    const Components &_eq,
                    const xReal       _eK,
                    const XReadable &_C,
                    const Level      _L);

            //! duplicate
            Outcome(const Outcome &) noexcept;

            //! cleanup
            ~Outcome() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            xReal activity(XMul &xmul, const xReal xi) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Situation   st; //!< current situation
            const Components &eq; //!< used components
            const xReal       eK; //!< used constant
            const XReadable  &C;  //!< initial concentration
            const Level       L;  //!< level

        private:
            Y_DISABLE_ASSIGN(Outcome);
        };

      

    }

}

#endif

