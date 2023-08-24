
//! \file

#ifndef Y_Lexical_Unit_Included
#define Y_Lexical_Unit_Included 1

#include "y/jive/token.hpp"


namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            class Unit : public Object, public Context, public Token
            {
            public:
                explicit Unit(const Context &) noexcept;
                Unit(const Unit &);
                virtual ~Unit() noexcept;
                
                Unit *next;
                Unit *prev;
                
            private:
                Y_DISABLE_ASSIGN(Unit);
            };
            
            typedef CxxListOf<Unit> Units;
            
        }

    }
}

#endif
