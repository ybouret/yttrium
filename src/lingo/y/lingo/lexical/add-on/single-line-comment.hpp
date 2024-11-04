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
            //__________________________________________________________________
            //
            //
            //
            //! Single Line Comment, returning on new line
            //
            //
            //__________________________________________________________________
            class SingleLineComment : public AddOn
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const EndlExpr; //!< "[:endl:]"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! triggered by enterExpr, back on new line, drop dot.
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

                //! cleanup
                virtual ~SingleLineComment() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleLineComment);
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

            //__________________________________________________________________
            //
            //
            //
            // some examples/utils
            //
            //
            //__________________________________________________________________
            Y_Lingo_Lexical_SingleLineComment(Shell,     '#');    //!< Shell style comment
            Y_Lingo_Lexical_SingleLineComment(CPlusPlus, "//");   //!< C++ comment
            Y_Lingo_Lexical_SingleLineComment(Lua,       "--");   //!< Lua comment

        }

    }

}

#endif

