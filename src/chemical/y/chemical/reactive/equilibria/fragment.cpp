
#include "y/chemical/reactive/equilibria/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fragment::  Fragment() noexcept :  EList(), species() {}
        Fragment:: ~Fragment() noexcept {}
        Fragment::  Fragment(const Fragment &sub) : EList(sub), species(sub.species) {}
    }

}


