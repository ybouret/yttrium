
#include "y/csv/field.hpp"

namespace Yttrium
{

    namespace CSV
    {
        Field:: Field() : text(), kind(Generic)
        {
        }

        Field:: ~Field() noexcept
        {
        }

        Field:: Field(const Field &other) : text(other.text), kind(other.kind) {}

    }

}


