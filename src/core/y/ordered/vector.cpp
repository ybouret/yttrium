
#include "y/ordered/vector.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Core
    {

        OrderedVector::  OrderedVector() noexcept {}
        OrderedVector:: ~OrderedVector() noexcept {}

        const char * const OrderedVector:: CallSign = "OrderedVector";


        void OrderedVector:: multipleValueException() const
        {
            throw Specific::Exception(CallSign,"trying to insert multiple value");
        }

        void OrderedVector:: valueNotFoundException() const
        {
            throw Specific::Exception(CallSign,"trying to remove missing value");
        }
    }
}
