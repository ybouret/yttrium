
#include "y/associative/little-endian-addressed.hpp"
#include "y/object.hpp"
#include "y/ptr/auto.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace {

    typedef LittleEndianAddressed LEAK;

    class Derived : public Object, public LEAK
    {
    public:
        explicit Derived()
        {
        }

        virtual ~Derived() noexcept
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Derived);
    };


}

Y_UTEST(associative_leak)
{
    
    AutoPtr<LEAK> ptr = new Derived();
    std::cerr << "ptr.leak@" << ptr->leak << std::endl;
}
Y_UDONE()
