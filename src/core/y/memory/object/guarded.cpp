#include "y/memory/object/guarded.hpp"

namespace Yttrium
{
    namespace Memory
    {

        ObjectGuarded:: ObjectGuarded(const ObjectLocator &objloc, ObjectLocator::Query query) noexcept :
        blockSize(0), blockAddr((objloc.*query)(Coerce(blockSize)))
        {

        }

        ObjectGuarded::  ~ObjectGuarded() noexcept
        {

        }


    }

}
