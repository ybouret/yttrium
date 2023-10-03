
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
            //__________________________________________________________________
            //
            //
            //
            //! Single Line Comment (interruptible by EOS)
            //
            //
            //__________________________________________________________________
            class SingleLineComment : public Plugin
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~SingleLineComment() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //! setup, returning on newline, discarding otherwise
                /**
                 \param lx parent lexer/scanner
                 \param id plugin's name
                 \param rx regular expression triggering call to plugin
                 */
                //______________________________________________________________
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

            //__________________________________________________________________
            //
            //
            //! helper to create single line comments
            //
            //__________________________________________________________________
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

            Y_JIVE_SINGLE_LINE_COMMENT(CxxComment,     "//"); //!< C++ comment
            Y_JIVE_SINGLE_LINE_COMMENT(HashTagComment, '#');  //!< Shell/Julia comment
            Y_JIVE_SINGLE_LINE_COMMENT(LuaComment,     "--"); //!< Lua Comments

        }

    }

}

#endif

