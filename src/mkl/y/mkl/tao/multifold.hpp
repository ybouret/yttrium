
//! \file

#ifndef Y_MKL_TAO_Multifold_Included
#define Y_MKL_TAO_Multifold_Included 1

#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {

            template <typename T>
            class DynamicAdd : public Object, public Antelope::Add<T>
            {
            public:
                inline explicit DynamicAdd() {}
                inline virtual ~DynamicAdd() noexcept {}

                DynamicAdd *next;
                DynamicAdd *prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(DynamicAdd);
            };

            template <typename T>
            class Multifold
            {
            };

        }

    }

}

#endif
