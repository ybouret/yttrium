
//! \file

#ifndef Y_Lingo_Lexical_SingleLineComment_Included
#define Y_Lingo_Lexical_SingleLineComment_Included 1

#include "y/lingo/lexical/add-on.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            class SingleLineComment : public AddOn
            {
            public:
                static const char * const EndlExpr;

                template <
                typename LABEL,
                typename ENTER
                > inline
                explicit SingleLineComment(Lexer       & rootLexer,
                                           const LABEL & addOnUUID,
                                           const ENTER & enterExpr) :
                AddOn(rootLexer,
                      addOnUUID,
                      enterExpr,
                      EndlExpr,
                      Unit::Endl,
                      AcceptEndOfSource)
                {
                    setup();
                }
                

                virtual ~SingleLineComment() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleLineComment);
                void setup();
            };


#define Y_Lingo_Lexical_SingleLineComment(NAME,EXPR)  \
/**/  class NAME##Comment : public SingleLineComment  \
/**/    {                                             \
/**/    public:                                       \
/**/      template <typename LABEL>                   \
/**/      inline explicit NAME##Comment(Lexer &rootLexer, const LABEL &addOnUUID) : SingleLineComment(rootLexer,addOnUUID,EXPR) {} \
/**/      inline virtual ~NAME##Comment() noexcept {} \
/**/    private:                                      \
/**/      Y_DISABLE_COPY_AND_ASSIGN(NAME##Comment);   \
/**/     }

            Y_Lingo_Lexical_SingleLineComment(Shell, '#');

        }

    }

}

#endif

