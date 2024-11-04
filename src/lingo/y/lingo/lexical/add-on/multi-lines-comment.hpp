//! \file

#ifndef Y_Lingo_Lexical_MultiLinesComment_Included
#define Y_Lingo_Lexical_MultiLinesComment_Included 1

#include "y/lingo/lexical/add-on.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //
            //! Multi Lines Comment
            //
            //
            //__________________________________________________________________
            class MultiLinesComment : public AddOn
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! triggered by enterExpr, back on leaveExpr, drop dot, endl
                template <
                typename LABEL,
                typename ENTER,
                typename LEAVE
                > inline
                explicit MultiLinesComment(Lexer       & rootLexer,
                                           const LABEL & addOnUUID,
                                           const ENTER & enterExpr,
                                           const LEAVE & leaveExpr) :
                AddOn(rootLexer,
                      addOnUUID,
                      enterExpr,
                      leaveExpr,
                      Unit::Bulk,
                      RejectEndOfSource)
                {
                    setup();
                }

                //! cleanup
                virtual ~MultiLinesComment() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MultiLinesComment);
                void setup();
            };

            //__________________________________________________________________
            //
            //
            //
            //! helper to created Lexical::NAMEComment
            //
            //
            //__________________________________________________________________
#define Y_Lingo_Lexical_MultiLinesComment(NAME,INI,END)  \
/**/  class NAME##Comment : public MultiLinesComment     \
/**/    {                                                \
/**/    public:                                          \
/**/      template <typename LABEL>                      \
/**/      inline explicit NAME##Comment(Lexer &rootLexer, const LABEL &addOnUUID) : MultiLinesComment(rootLexer,addOnUUID,INI,END) {} \
/**/      inline virtual ~NAME##Comment() noexcept {}    \
/**/    private:                                         \
/**/      Y_DISABLE_COPY_AND_ASSIGN(NAME##Comment);      \
/**/     }


            Y_Lingo_Lexical_MultiLinesComment(C_, "/\\*","\\*/");

        }

    }

}

#endif

