

//! \file

#ifndef Y_Chemical_Formula_Linker_Included
#define Y_Chemical_Formula_Linker_Included 1

#include "y/chemical/formula.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/lingo/syntax/translator.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Formula::Linker : public Lingo::Syntax::Translator
        {
        public:
            explicit Linker(const Weasel::Parser &);
            virtual ~Linker() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Linker);
        };
    }

}

#endif

