
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
        //
        //! Outcome of an Aftermath
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

            //! setup with all information from Aftermath
            Outcome(const Situation    _st,
                    const Components & _eq,
                    const xreal_t      _eK,
                    const XReadable &  _cc,
                    const Level     &  _lv,
                    const xreal_t      _xi) noexcept;
            Outcome(const Outcome &_)        noexcept; //!< duplicat
            ~Outcome()                       noexcept; //!< cleanup


            const char * situation() const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Situation    st; //!< predicted situation
            const Components & eq; //!< the components
            const xreal_t      eK; //!< current constant
            const XReadable  & cc; //!< predicted concentrations mathching eK
            const Level        lv; //!< concentrations level
            const xreal_t      xi; //!< extent from original concentrations
        private:
            Y_DISABLE_ASSIGN(Outcome);
        };

    }

}

#endif

