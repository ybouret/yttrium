

//! \file

#ifndef Y_Lingo_Lexical_Unit_Included
#define Y_Lingo_Lexical_Unit_Included 1

#include "y/lingo/token.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            class Unit : public Object,  public Entity, public Token
            {
            public:
                explicit Unit(const Caption &,
                              const Context &) noexcept;
                virtual ~Unit() noexcept;

                Unit *        next;
                Unit *        prev;
                const Context info;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Unit);
            };
        }

        typedef Lexical::Unit     Lexeme;
        typedef CxxListOf<Lexeme> Lexemes;
    }

}

#endif

