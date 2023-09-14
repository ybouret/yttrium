//! \file

#ifndef Y_Jive_Pattern_Matcher_Included
#define Y_Jive_Pattern_Matcher_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/jive/source.hpp"


namespace Yttrium
{
    namespace Jive
    {
        class Dictionary;


        class Matcher
        {
        public:

            virtual ~Matcher() noexcept;
            Matcher(Pattern *p) noexcept;
            Matcher(const char   *, const Dictionary * = 0);
            Matcher(const String &, const Dictionary * = 0);
            Matcher(const Matcher &); //!< clone pattern, copy first chars


            void    cleanUp() noexcept;
            Token * exactly(Source &source);
            Token * somehow(Source &source);

            const Motif      motif;
            const FirstChars firstChars;

        private:
            Y_DISABLE_ASSIGN(Matcher);

            void initialize();
        };

    }
}

#endif

