
#include "y/jive/pattern/regexp/compiler.hpp"
#include "y/jive/pattern/posix.hpp"

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
            fillPosixDict(posixDict);
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
            inline RXC(const char  *rx,
                       const size_t sz,
                       const Dictionary &pd,
                       const Dictionary *ud) noexcept :
            expr(rx),
            curr(expr),
            last(expr+sz),
            posixDict(pd),
            userDictP(ud)
            {
            }

            inline ~RXC() noexcept
            {
            }



            const char * const expr;
            const char *       curr;
            const char * const last;
            const Dictionary  &posixDict;
            const Dictionary  *userDictP;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RXC);
        };


        Pattern * RegExpCompiler:: operator()(const String &rx, const Dictionary *dict) const
        {
            RXC(rx.c_str(),rx.size(),posixDict,dict);
            return 0;
        }

    }

}

