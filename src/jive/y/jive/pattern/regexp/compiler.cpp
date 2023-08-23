
#include "y/jive/pattern/regexp/compiler.hpp"
#include "y/jive/pattern/posix.hpp"
#include "y/jive/pattern/all.hpp"
#include "y/system/exception.hpp"
#include "y/text/ascii/printable.hpp"
#include "y/text/hexadecimal.hpp"

#include <cstring>

namespace  Yttrium
{
    namespace Jive
    {

        const char * const RegExpCompiler:: CallSign = "RegExpCompiler";

#define Y_RX_POSIX(NAME) dict(#NAME, posix:: NAME() )

        static inline void fillPosixDict(Dictionary &dict)
        {
            Y_RX_POSIX(lower);
            Y_RX_POSIX(upper);
            Y_RX_POSIX(alpha);
            Y_RX_POSIX(digit);
            Y_RX_POSIX(alnum);
            Y_RX_POSIX(xdigit);
            Y_RX_POSIX(blank);
            Y_RX_POSIX(space);
            Y_RX_POSIX(punct);
            Y_RX_POSIX(word);
            Y_RX_POSIX(endl);
            Y_RX_POSIX(dot);
            Y_RX_POSIX(core);
            Y_RX_POSIX(vowel);
            Y_RX_POSIX(consonant);
        }

        RegExpCompiler:: RegExpCompiler() : Singleton<RegExpCompiler>(), posixDict()
        {
            fillPosixDict(Coerce(posixDict));
        }


        RegExpCompiler:: ~RegExpCompiler() noexcept
        {
        }

        

    }
}

namespace  Yttrium
{
    namespace Jive
    {

        class RXC
        {
        public:
            static const char LPAREN    =  '(';
            static const char RPAREN    =  ')';
            static const char LBRACK    =  '[';
            static const char RBRACK    =  ']';
            static const char ALT       =  '|';
            static const char BACKSLASH = '\\';
            static const char QUOTE     = '\'';
            static const char DQUOTE    = '\"';
            static const char LBRACE    =  '{';
            static const char RBRACE    =  '}';
            static const char CARET     =  '^';
            static const char MINUS     =  '-';
            static const char DOT       =  '.';
            static const char COLON     =  ':';

            static const char * const CallSign;

            typedef Pattern * (*MakeJoker)(Pattern *);

            //! initialize
            inline RXC(const char       *rx,
                       const size_t      sz,
                       const Dictionary &pd,
                       const Dictionary *ud) noexcept :
            expr(rx), curr(expr), last(expr+sz), deep(0), posixDict(pd), userDictP(ud)
            {
            }

            //! cleanup
            inline ~RXC() noexcept { }


            const char * const expr;      //!< original expression
            const char *       curr;      //!< current position
            const char * const last;      //!< end of expression
            int                deep;      //!< depth
            const Dictionary  &posixDict; //!< RexExpCompiler
            const Dictionary  *userDictP; //!< optional


#include "sub-expr.hxx"
#include "make-jkr.hxx"
#include "esc-expr.hxx"
#include "esc-band.hxx"
#include "esc-hexa.hxx"
#include "to-posix.hxx"
#include "sub-band.hxx"
#include "to-range.hxx"






        private:
            Y_DISABLE_COPY_AND_ASSIGN(RXC);
        };

        const char * const RXC::CallSign = RegExpCompiler::CallSign;

        
        Pattern * RegExpCompiler:: compile(const String &rx, const Dictionary *dict) const
        {
            RXC rxc(rx.c_str(),rx.size(),posixDict,dict);
            AutoPtr<Pattern>  result = rxc.subExpr();
            if(rxc.deep!=0)
                throw Specific::Exception(CallSign,"unfinished expression in '%s'",rx());
            return result.yield();
        }

    }

}

