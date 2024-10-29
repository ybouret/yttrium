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

            
            class Outcome
            {
            public:

                const Unit::Type type;
                const Unit::Spot spot;
                const union {
                    Unit::RegularInfo regular;
                    Unit::ControlInfo control;
                } args;

                Outcome(const Unit::Feat,const Unit::Spot) noexcept; //!< regular outcome
                Outcome(const Unit::Spot)                  noexcept; //!< control outcome
                Outcome(const Outcome &)                   noexcept; //!< copy
                ~Outcome()                                 noexcept; //!< cleanup

            private:
                Y_DISABLE_ASSIGN(Outcome);
            };
        }

    }

}

#endif

