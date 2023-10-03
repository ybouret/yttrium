
//! \file

#ifndef Y_Jive_Lexical_Single_Line_Comment_Included
#define Y_Jive_Lexical_Single_Line_Comment_Included 1

#include "y/jive/lexical/plugin.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            class SingleLineComment : public Plugin
            {
            public:
                virtual ~SingleLineComment() noexcept;

                template <typename IDENTIFIER, typename ENTER_EXPR> inline
                explicit SingleLineComment(Lexer            &lx,
                                           const IDENTIFIER &id,
                                           const ENTER_EXPR &rx) :
                Plugin(lx,id,rx,AcceptEndOfStream) { setup(); }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleLineComment);
                void            setup();
                virtual Message enter(const Token &) noexcept; //!< return 0
                virtual Message leave(const Token &) noexcept; //!< return LX_ENDL
            };

#define Y_JIVE_SINGLE_LINE_COMMENT(NAME,EXPR)             \
class NAME : public Jive::Lexical::SingleLineComment {    \
/**/ public:                                              \
/**/     inline virtual ~NAME() noexcept {}               \
/**/     template <typename IDENTIFIER> inline            \
/**/     explicit NAME(Lexer &lx, const IDENTIFIER &id) : \
/**/     Jive::Lexical::SingleLineComment(lx,id,EXPR) {}  \
/**/ private:                                             \
/**/     Y_DISABLE_COPY_AND_ASSIGN(NAME);                 \
}

            Y_JIVE_SINGLE_LINE_COMMENT(CxxComment, "//");

        }

    }

}

#endif

