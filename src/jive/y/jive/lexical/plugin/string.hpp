
//! \file

#ifndef Y_Jive_Lexical_String_Included
#define Y_Jive_Lexical_String_Included 1

#include "y/jive/lexical/plugin.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //
            //! base class for Lexical String
            //
            //
            //__________________________________________________________________
            class String_ : public Plugin
            {
            public:
                //__________________________________________________________________
                //
                //
                // Definitions
                //
                //__________________________________________________________________
                static const char * const Com; //!< common 7-bits chars

                //! Kind of string
                enum Kind
                {
                    DQUOTES, //!< double quotes separated
                    QUOTES,  //!< single quotes separated
                    BRACKETS //!< braket separated
                };

                //__________________________________________________________________
                //
                //
                // C++
                //
                //__________________________________________________________________

                //! cleanup
                virtual ~String_() noexcept;

            protected:
                //! setup
                template <typename ID> inline
                explicit String_(Lexer       &lx,
                                 const ID    &id,
                                 const Kind   sk) :
                Plugin(lx,id,GetEntryRX(sk),RejectEndOfStream),
                content()
                {
                    initialize(sk);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(String_);
                static const char *   GetEntryRX(const Kind) noexcept;
                void                  initialize(const Kind sk);
                virtual Message       enter(const Token &);
                virtual Message       leave(const Token &);
                Message               onCom(const Token &);

                Token content;
            };

        }

    }

}

#endif
