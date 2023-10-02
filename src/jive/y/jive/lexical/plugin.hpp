
//! \file

#ifndef Y_Jive_Lexical_Plugin_Included
#define Y_Jive_Lexical_Plugin_Included 1

#include "y/jive/lexical/scanner.hpp"

namespace Yttrium
{
    //class XMLog;
    class Lexer;

    namespace Jive
    {

        namespace Lexical
        {
            class Plugin : public Scanner
            {
            public:
                virtual ~Plugin() noexcept;

                template <
                typename IDENTIFIER,
                typename ENTER_EXPR> inline
                explicit Plugin(Lexer             &lx,
                                const IDENTIFIER  &id) :
                Scanner(id),
                lexer(lx)
                {
                    
                }

                Lexer &lexer;

                Message enter(const Token & ) { return LX_DROP; }
                void    onLeave(const Token &token);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }

    }

}

#endif

