

//! \file

#ifndef Y_Jive_Lexical_Mutli_Lines_Comment_Included
#define Y_Jive_Lexical_Mutli_Lines_Comment_Included 1

#include "y/jive/lexical/plugin.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            class MultiLinesComment : public Plugin
            {
            public:
                virtual ~MultiLinesComment() noexcept;

                template <
                typename IDENTIFIER,
                typename ENTER_EXPR,
                typename LEAVE_EXPR> inline
                explicit MultiLinesComment(Lexer            &lx,
                                           const IDENTIFIER &id,
                                           const ENTER_EXPR &erx,
                                           const LEAVE_EXPR &lrx) :
                Plugin(lx,id,erx,RejectEndOfStream)
                { ret(lrx); setup(); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiLinesComment);
                void setup();
                virtual Message enter(const Token &) noexcept;
                virtual Message leave(const Token &) noexcept;

            };

#if 0
#define Y_JIVE_MULTI_LINES_COMMENT(NAME,EXPR)             \
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
#endif

        }

    }

}

#endif

