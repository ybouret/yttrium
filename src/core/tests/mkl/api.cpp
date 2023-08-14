#include "../main.hpp"

#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace Yttrium;


template <typename T>
static inline void TestFabs(Random::Bits &ran)
{
    std::cerr << '<' << typeid(T).name() << '>' << std::endl;
    for(size_t i=0;i<8;++i)
    {
        const T x = Bring<T>::Get(ran);
        const T a = MKL::Fabs<T>::Of(x);
        std::cerr << x << " -> " << a << std::endl;
    }
    std::cerr << std::endl;
}

Y_UTEST(mkl_api)
{
    Random::Rand ran;

    TestFabs<float>(ran);
    TestFabs<double>(ran);
    TestFabs<long double>(ran);

    TestFabs< XReal<float> >(ran);
    TestFabs< XReal<double> >(ran);
    TestFabs< XReal<long double> >(ran);

    TestFabs< apz >(ran);
    TestFabs< apq >(ran);

    TestFabs< Complex<float> >(ran);
    TestFabs< Complex<double> >(ran);
    TestFabs< Complex<long double> >(ran);

    {
        float a=3,b=4;
        float c = MKL::Hypotenuse(a,b);
        std::cerr << c << std::endl;
    }

    {
        XReal<float> a=3, b=4;
        XReal<float> c = MKL::Hypotenuse(a,b);
        std::cerr << c << "=" << float(c) << std::endl;
    }

    TestFabs< Complex< XReal<float> > >(ran);
    TestFabs< Complex< XReal<double> > >(ran);
    TestFabs< Complex< XReal<long double> > >(ran);

}
Y_UDONE()

