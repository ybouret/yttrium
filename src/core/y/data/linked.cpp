
#include "y/data/linked.hpp"

namespace Yttrium
{

    namespace Core
    {
        Linked::  Linked() noexcept : size(0) {}
        Linked:: ~Linked() noexcept { assert(0==size); }
    }

}


