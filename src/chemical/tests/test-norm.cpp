#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/api.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/indexing.hpp"

using namespace Yttrium;

namespace {

    class Sum
    {
    public:
        Sum(const size_t num, Random::Bits &ran) :
        n(num),
        a(n,0),
        b(n,0),
        z(n,0)
        {
            for(size_t i=n;i>0;--i)
            {
                {
                    double A = 1 + 4 * ran.to<double>();
                    if(ran.choice()) A = -A;
                    Coerce(a[i]) = A;
                }
                Coerce(b[i]) = 8 * ran.to<double>();
                Coerce(z[i]) = -b[i]/a[i];
            }

            std::cerr << "a=" << a << std::endl;
            std::cerr << "b=" << b << std::endl;
            std::cerr << "z=" << z << std::endl;

        }

        ~Sum() noexcept {}

        const size_t         n;
        const Vector<double> a;
        const Vector<double> b;
        const Vector<double> z;
    };

}

Y_UTEST(norm)
{
    Random::ParkMiller ran;
    const size_t       n = 3;


    Sum sum(n,ran);




}
Y_UDONE()
