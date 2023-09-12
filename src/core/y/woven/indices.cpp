#include "y/woven/indices.hpp"

namespace Yttrium
{
    namespace WOVEn
    {
        Indices::  Indices(const size_t n) :
        Identifiable(), Collection(),
        BaseType(n) {}

        Indices:: ~Indices() noexcept {}

        Indices:: Indices(const Indices &indx) :
        Identifiable(), Collection(),
        BaseType(indx)
        {}
        
    }


}

