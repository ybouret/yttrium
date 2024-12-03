#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Assembly::  Assembly()                  noexcept : maxKeySize(0)            {}
        Assembly::  Assembly(const Assembly &_) noexcept : maxKeySize(_.maxKeySize) {}
        Assembly:: ~Assembly() noexcept { forget(); }


        void Assembly:: enroll(const Entity &entity) noexcept
        {
            const size_t tmp = entity.key().size();
            if(tmp>maxKeySize) Coerce(maxKeySize) = tmp;
        }

        void Assembly:: forget() noexcept
        {
            Coerce(maxKeySize) = 0;
        }


    }

}

