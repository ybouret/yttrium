
//! \file

#ifndef Y_Jive_Pattern_RegExp_Compiler_Included
#define Y_Jive_Pattern_RegExp_Compiler_Included 1

#include "y/jive/pattern/dictionary.hpp"
#include "y/singleton.hpp"

namespace  Yttrium
{
    namespace Jive
    {
        class RegExpCompiler : public Singleton<RegExpCompiler>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 10;

            Pattern *            operator()(const String &rx, const Dictionary *dict) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RegExpCompiler);
            friend class Singleton<RegExpCompiler>;
            explicit RegExpCompiler();
            virtual ~RegExpCompiler() noexcept;
            Dictionary posixDict;

        };
    }
}

#endif

