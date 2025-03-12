#include "y/chemical/type/indexed.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {


        Indexed:: ~Indexed() noexcept
        {
            memset( (void*)&indx[0], 0, sizeof(indx) );
        }

        Indexed:: Indexed(const String *s, const size_t i) noexcept :
        Entity(s),
        indx()
        {
            setup(i);
        }

        void Indexed:: setup(const size_t i) noexcept
        {
            for(size_t j=0;j<MaxLevel;++j)
            {
                Coerce(indx[j]) = i;
            }
        }

    }
}
