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
            //__________________________________________________________________
            //
            //
            //
            //! Plugin: a pre-defined scanner
            //
            //
            //__________________________________________________________________
            class Plugin : public Scanner
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Plugin() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //! setup plugin, sharing data with parent lexer/scanner
                /**
                 \param lx parent lexer
                 \param id plugin name for lexer's call
                 \param rx regular expression triggering plugin call
                 \param ep policy upon unfinished plugin execution
                 */
                //______________________________________________________________
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

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual Message enter(const Token &) = 0; //!< initialize plugin upon enter token
                virtual Message leave(const Token &) = 0; //!< finalize plugin upon leave token

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Lexer                   &lexer; //!< parent lexer

            protected:

                //! create a return from call upon regular expression
                template <typename RX> inline
                void ret(const RX &rx)
                { back(rx,lexer, *this, & Plugin::onLeave); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
                static Scanner &L2S(Lexer &) noexcept;
                Message onEnter(const Token &token); //!< wrapper to call enter
                Message onLeave(const Token &token); //!< wrapper to call leave
            };
        }

    }

}

#endif

