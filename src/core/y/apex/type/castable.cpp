#include "y/apex/type/castable.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {


        Castable::  Castable() noexcept : Number() {}
        Castable:: ~Castable() noexcept {}

        void Castable:: raiseCastOverflowFor(const char * const msg) const
        {
            throw Specific::Exception( callSign(), "Cast Overflow for '%s'", (0!=msg) ? msg : Core::Unknown);
        }

    }

}
