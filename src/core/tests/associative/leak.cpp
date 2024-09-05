
#include "y/associative/little-endian-addressed.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace {

    typedef LittleEndianAddressed LEAK;

}

Y_UTEST(associative_leak)
{
    {
        LEAK  obj;
        LEAK &ref = obj.as<LEAK>();
        Y_CHECK(&ref == &obj);
    }
}
Y_UDONE()
