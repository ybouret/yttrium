
//! \file

#ifndef Y_Chemical_Outcome_Included
#define Y_Chemical_Outcome_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

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
            Outcome(const Situation     _st,
                    const Equilibrium & _eq,
                    const xreal_t       _eK,
                    XWritable       &   _cc,
                    const Level     &   _lv,
                    const xreal_t       _xi) noexcept;
            Outcome(const Outcome &_)       noexcept; //!< duplicat
            ~Outcome()                      noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Outcome);                  //!< display

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const char * situation() const noexcept; //!< SituationText(st)

            //! evaluate affinity from given state
            xreal_t      affinity(XAdd &xadd, const XReadable &C, const Level L) const;

            //! evaluate activity from given state
            xreal_t      activity(XMul &xmul, const XReadable &C, const Level L) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Situation     st; //!< predicted situation
            const Equilibrium & eq; //!< the components
            const xreal_t       eK; //!< current constant
            XWritable  &        cc; //!< predicted concentrations mathching eK
            const Level         lv; //!< concentrations level
            xreal_t             xi; //!< extent from original concentrations
            xreal_t             ax; //!< |xi|
            xreal_t             sc; //!< score
        private:
            Y_DISABLE_ASSIGN(Outcome);
        };

    }

}

#endif

