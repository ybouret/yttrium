//! \file

#ifndef Y_JSON_Compiler_Included
#define Y_JSON_Compiler_Included 1

#include "y/json/value.hpp"
#include "y/lingo/module.hpp"

namespace Yttrium
{
    namespace JSON
    {
        class Compiler
        {
        public:
            explicit Compiler();
            virtual ~Compiler() noexcept;

            void load(Value &jv, Lingo::Module * const);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            class Code;
            Code *code;
        };
    }
}

#endif
