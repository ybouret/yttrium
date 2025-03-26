#include "y/chemical/weasel/formula/to-html.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Weasel:: FormulaToHTML:: ~FormulaToHTML() noexcept
        {
        }

        const char * const Weasel:: FormulaToHTML:: CallSign = "FormulaToHTML";

        Weasel:: FormulaToHTML:: FormulaToHTML() :
        FormulaCompiler(CallSign)
        {
        }

        const String * Weasel:: FormulaToHTML:: get(const XNode &node)
        {

            return new String();
        }



    }

}


