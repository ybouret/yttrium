#include "y/chem/lang/linker.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Linker:: ~Linker() noexcept
        {
        }

        Linker:: Linker() noexcept : Jive::Syntax::Translator()
        {
        }

        void Linker:: operator()(const XNode &root,
                                 Library     &lib,
                                 Equilibria  &eqs)
        {
            
        }

    }

}

