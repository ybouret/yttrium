#include "y/color/print.hpp"

namespace Yttrium
{
    namespace Color
    {
        void Print<uint8_t>::Out(std::ostream &os, const uint8_t x)
        {
            os << unsigned(x);
        }

        void Print<int8_t>::Out(std::ostream &os, const int8_t x)
        {
            os << int(x);
        }

    }
}
