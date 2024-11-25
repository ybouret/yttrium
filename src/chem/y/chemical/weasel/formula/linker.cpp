
#include "y/chemical/weasel/formula/linker.hpp"
#include "y/chemical/weasel/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Formula:: Linker:: ~Linker() noexcept
        {
        }

        Formula:: Linker:: Linker(const Weasel::Parser &parser) :
        Lingo::Syntax::Translator(parser.FORMULA.name)
        {
            
        }
    }

}
