#include "y/chemical/type/indexed.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        void Indexed:: clear() noexcept
        {
            memset( (void*)&indx[0], 0, sizeof(indx) );
        }

        Indexed:: ~Indexed() noexcept
        {
            clear();
        }

        Indexed:: Indexed(const String *s, const size_t i) noexcept :
        Entity(s),
        indx()
        {
            setup(i);
        }

        void Indexed:: setup(const size_t i) noexcept
        {
            clear();
            Coerce(indx[TopLevel]) = i;
        }

    }
}
