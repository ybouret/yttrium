
#include "y/object.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


namespace
{
    template <size_t N>
    class Derived : public Object
    {
    public:
        explicit Derived() noexcept : Object(), data() {}
        virtual ~Derived() noexcept {}

        char data[N];

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Derived);
    };
}

Y_UTEST(object)
{
    
    Object *obj = new Object();
    delete  obj;
    obj = 0;
    delete obj;

    Object *ptr[] = {
        new Derived<1>(),
        new Derived<2>(),
        new Derived<3>(),
        new Derived<4>(),
        new Derived<5>(),
        new Derived<6>(),
        new Derived<7>(),
        new Derived<8>(),
        new Derived<10>(),
        new Derived<20>()
    };
    for(size_t i=0;i<sizeof(ptr)/sizeof(ptr[0]);++i)
    {
        delete ptr[i];
    }


}
Y_UDONE()

