#include "y/oversized.hpp"
#include "y/utest/run.hpp"
#include "y/random/fill.hpp"

using namespace Yttrium;

namespace
{
    template <size_t N>
    class Dummy : public Oversized
    {
    public:
        explicit Dummy() : data() {}
        virtual ~Dummy() noexcept {}

        char data[N];

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dummy);
    };

    template <size_t N> static inline
    void testDummy(Random::Bits &ran)
    {
        typedef Dummy<N> MyDummy;
        std::cerr << "N=" << N << std::endl;
        Y_SIZEOF(MyDummy);

        MyDummy *dum = new MyDummy();

        Random::Fill::Block(dum->data,N,ran,0x1);

        delete dum;

    }

};


Y_UTEST(oversized)
{
    Random::Rand ran;
    testDummy<1>(ran);
    testDummy<2>(ran);
    testDummy<3>(ran);
    testDummy<4>(ran);
    testDummy<5>(ran);
    testDummy<6>(ran);
    testDummy<7>(ran);
    testDummy<8>(ran);
    testDummy<9>(ran);
    testDummy<10>(ran);
    testDummy<100>(ran);
    testDummy<1000>(ran);


}
Y_UDONE()

