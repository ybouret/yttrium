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
            enum EndOfStreamPolicy
            {
                RejectEndOfStream,
                AcceptEndOfStream
            };

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
                                const EndOfStreamPolicy &ep) :
                Scanner(id),
                parent(lx),
                policy(ep)
                {
                    L2S(parent).call(name, rx, parent, *this, & Plugin::onEnter );
                }

                Message onEnter(const Token &token)
                {
                    return 0;
                }

                Lexer                   &parent;
                const EndOfStreamPolicy  policy;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
                static Scanner &L2S(Lexer &) noexcept;
            };
        }

    }

}

#endif

