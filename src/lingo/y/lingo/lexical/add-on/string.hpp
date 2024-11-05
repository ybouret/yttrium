//! \file

#ifndef Y_Lingo_Lexical_String_Included
#define Y_Lingo_Lexical_String_Included 1

#include "y/lingo/lexical/add-on.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            
            class String_ : public AddOn
            {
            public:
                virtual ~String_() noexcept;

            protected:
                template <
                typename LABEL> inline
                explicit String_(Lexer       & rootLexer,
                                 const LABEL & addOnUUID,
                                 const char    enterChar,
                                 const char    leaveChar) :
                AddOn(rootLexer,
                      addOnUUID,
                      enterChar,
                      leaveChar,
                      Lexeme::Bulk,
                      RejectEndOfSource),
                content()
                {
                    setup(enterChar,leaveChar);
                }


                Token content;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(String_);
                virtual void enter(const Token &); //!< clear content, append token
                virtual void leave(const Token &); //!< append token, produce unit
                void         setup(const char enterChar, const char leaveChar); //!< setup

                Outcome onCore(const Token &);   //!< direct add
                Outcome onEscRaw(const Token &); //!< '\\' + char => add char
                Outcome onEscCtl(const Token &); //!< '\\' + char => coding char


            };
        }

    }

}

#endif

