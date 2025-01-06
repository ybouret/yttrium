

#include "y/chemical/plexus/type/cursor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Cursor:: Cursor(const Species &_first,
                        const xReal    _value,
                        const SBank &  _sbank) :
        Marker(_sbank)
        {

            srepo << _first;
            value =  _value;
        }

        Cursor:: ~Cursor() noexcept
        {
        }

        Cursor:: Cursor(const Cursor &_) :
        Marker(_)
        {
        }



#if 0
        xReal & Cursor:: operator*() noexcept
        {
            return value;
        }
#endif
        
        const xReal & Cursor:: operator*() const noexcept
        {
            return value;
        }

        void Cursor:: join(const Species &s)
        {
            assert( !srepo.has(s) );
            srepo << s;
        }

    }

}

