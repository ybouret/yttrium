
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
            //! ...
            //
            //
            //__________________________________________________________________
            class String_ : public Plugin
            {
            public:
                static const char * const Com;;
                
                enum Kind
                {
                    DQUOTES,
                    QUOTES,
                    BRACKETS
                };
                virtual ~String_() noexcept;

            protected:
                template <typename ID> inline
                explicit String_(Lexer       &lx,
                                 const ID    &id,
                                 const Kind   sk) :
                Plugin(lx,id,GetEntryRX(sk),RejectEndOfStream)
                {
                    initialize(sk);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(String_);
                static const char *   GetEntryRX(const Kind) noexcept;
                void                  initialize(const Kind sk);

                virtual Message enter(const Token &);
                virtual Message leave(const Token &);
                Message         onCom(const Token &);

                Token content;
            };

        }

    }

}

#endif
