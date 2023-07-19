#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer::  Integer() : s(__Zero__), n() {}
        Integer:: ~Integer() noexcept {}

        Integer:: Integer(const Integer &z) : s(z.s), n(z.n) {}

        Integer & Integer:: operator=(const Integer &other)
        {
            Integer tmp(other);
            xch(tmp);
            return *this;
        }


        void Integer:: xch(Integer &z) noexcept
        {
            Coerce(n).xch( Coerce(z.n) );
            CoerceSwap(s,z.s);
        }


    }
}
