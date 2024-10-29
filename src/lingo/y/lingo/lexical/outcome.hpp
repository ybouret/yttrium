//! \file

#ifndef Y_Lingo_Lexical_Outcome_Included
#define Y_Lingo_Lexical_Outcome_Included 1

#include "y/lingo/lexical/unit.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //
            //! Outcome from Scanner run
            //
            //
            //__________________________________________________________________
            class Outcome
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Outcome(const Unit::Feat,const Unit::Spot) noexcept; //!< regular outcome
                Outcome(const Unit::Spot)                  noexcept; //!< control outcome
                Outcome(const Outcome &)                   noexcept; //!< copy
                ~Outcome()                                 noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Unit::Type type; //!< type
                const Unit::Spot spot; //!< spot
                const union {
                    Unit::RegularInfo regular;
                    Unit::ControlInfo control;
                } args;                //!< args

            private:
                Y_DISABLE_ASSIGN(Outcome);
            };
        }

    }

}

#endif

