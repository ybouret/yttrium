

#include "y/chemical/plexus/cursor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Cursor:: Cursor(const Species &_first,
                        const xReal    _value,
                        const SBank &  _sbank) :
        SRepo(_sbank),
        value(_value)
        {
            (*this) << _first;
        }

        Cursor:: ~Cursor() noexcept
        {
        }

        Cursor:: Cursor(const Cursor &_) :
        SRepo(_),
        value(_.value)
        {
        }

    }

}

