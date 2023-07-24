
#include "y/container/collection.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    Collection:: Collection() noexcept
    {
    }

    Collection:: ~Collection() noexcept
    {

    }

    unit_t Collection:: ssize() const
    {
        const unit_t res = static_cast<unit_t>( size() );
        if( res < 0 ) throw Specific::Exception( callSign(), "ssize overflow");
        return res;
    }

}
