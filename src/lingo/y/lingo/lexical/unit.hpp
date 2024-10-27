

//! \file

#ifndef Y_Lingo_Lexical_Unit_Included
#define Y_Lingo_Lexical_Unit_Included 1

#include "y/lingo/token.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Unit : public Token
        {
        public:

            Unit *next;
            Unit *prev;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Unit);
        };

    }

}

#endif

