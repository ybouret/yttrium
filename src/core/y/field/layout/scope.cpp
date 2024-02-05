
#include "y/field/layout/scope.hpp"
#include "y/mkl/v3d.hpp"

namespace Yttrium
{

    namespace Field
    {

        static const size_t data[8] = { 0, 0, 0, 0, 0, 0, 0, 0};


        const void * ZeroCount:: Entry() noexcept
        {
            return data;
        }

        size_t ZeroCount:: Bytes() noexcept
        {
            return sizeof(data);
        }
    }

}

