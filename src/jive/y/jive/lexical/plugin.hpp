//! \file

#ifndef Y_Jive_Lexical_Plugin_Included
#define Y_Jive_Lexical_Plugin_Included 1

#include "y/jive/lexical/scanner.hpp"

namespace Yttrium
{

    namespace Jive
    {
        class Lexer;
        
        namespace Lexical
        {

            class Plugin : public Scanner
            {
            public:
                virtual ~Plugin() noexcept;

                template <
                typename IDENTIFIER,
                typename ENTER_EXPR> inline
                explicit Plugin(Lexer                   &lx,
                                const IDENTIFIER        &id,
                                const ENTER_EXPR        &rx,
                                const EndOfStreamPolicy  ep) :
                Scanner(id,L2S(lx),ep),
                lexer(lx)
                {
                    assert(0!=root);
                    root->call(name, rx, lexer, *this, & Plugin::onEnter );
                }

                virtual Message enter(const Token &) = 0;
                virtual Message leave(const Token &) = 0;


                Lexer                   &lexer;

            protected:
                template <typename RX> inline
                void ret(const RX &rx)
                { back(rx,lexer, *this, & Plugin::onLeave); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
                static Scanner &L2S(Lexer &) noexcept;
                Message onEnter(const Token &token);
                Message onLeave(const Token &token);
            };
        }

    }

}

#endif

