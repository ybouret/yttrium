
#include "y/chemical/weasel/formula/compiler.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        FormulaCompiler:: ~FormulaCompiler() noexcept
        {
        }

        FormulaCompiler:: FormulaCompiler(const char * const which) :
        Lingo::Syntax::Translator(which)
        {
            
        }

    }

}
