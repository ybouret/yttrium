#include "y/object.hpp"
#include "y/random/fill.hpp"
#include "y/random/shuffle.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


namespace
{
    template <size_t N>
    class Derived : public Object
    {
    public:
        explicit Derived(Random::Bits &ran) noexcept : Object(), data()
        {
            std::cerr << "+Derived<" << N << ">" << std::endl;
            Random::Fill::Block(data, sizeof(data), ran,1);
        }

        virtual ~Derived() noexcept
        {
            std::cerr << "-Derived<" << N << ">" << std::endl;
        }

        char data[N];

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Derived);
    };
}

Y_UTEST(object)
{
    Random::Rand ran;

    {
        Object *obj = new Object();
        delete  obj;
        obj = 0;
        delete obj;
    }

    {
        Object *ptr[] = {
            new Derived<1>(ran),
            new Derived<2>(ran),
            new Derived<3>(ran),
            new Derived<4>(ran),
            new Derived<5>(ran),
            new Derived<6>(ran),
            new Derived<7>(ran),
            new Derived<8>(ran),
            new Derived<10>(ran),
            new Derived<20>(ran)
        };

        Random::Shuffle::Range(ptr,sizeof(ptr)/sizeof(ptr[0]),ran);

        for(size_t i=0;i<sizeof(ptr)/sizeof(ptr[0]);++i)
        {
            delete ptr[i];
        }
    }

    {
        Object *arr = new Object[4];
        delete []arr;
    }


}
Y_UDONE()

