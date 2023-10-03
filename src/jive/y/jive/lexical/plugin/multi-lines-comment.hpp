

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

            //__________________________________________________________________
            //
            //
            //
            //! Multi Lines Comment between delimiter expressions
            //
            //
            //__________________________________________________________________
            class MultiLinesComment : public Plugin
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~MultiLinesComment() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //! initialize, counting newlines and discarding other
                /**
                 \param lx parent lexer/scanner
                 \param id plugin's name
                 \param erx entry on regular expression
                 \param lrx leave on regular expression
                 */
                //______________________________________________________________
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

            //__________________________________________________________________
            //
            //
            //! helper to create multi-lines comment
            //
            //__________________________________________________________________
#define Y_JIVE_MULTI_LINES_COMMENT(NAME,ENTER,LEAVE)             \
class NAME : public Jive::Lexical::MultiLinesComment {           \
/**/ public:                                                     \
/**/     inline virtual ~NAME() noexcept {}                      \
/**/     template <typename IDENTIFIER> inline                   \
/**/     explicit NAME(Lexer &lx, const IDENTIFIER &id) :        \
/**/     Jive::Lexical::MultiLinesComment(lx,id,ENTER,LEAVE) {}  \
/**/ private:                                                    \
/**/     Y_DISABLE_COPY_AND_ASSIGN(NAME);                        \
}

            Y_JIVE_MULTI_LINES_COMMENT(C_Comment,   "/\\*","\\*/"); //!< C   Comment
            Y_JIVE_MULTI_LINES_COMMENT(XML_Comment, "<!--","-->");  //!< XML Comment

        }

    }

}

#endif

