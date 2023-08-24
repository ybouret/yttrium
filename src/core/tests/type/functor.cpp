#include "y/functor.hpp"
#include "y/utest/run.hpp"
#include <cmath>

using namespace Yttrium;

static inline double Sin_(double x) { return sin(x); }

namespace
{
    class MulBy
    {
    public:
        MulBy(const int a) :
        fac(a)
        {
            std::cerr << "+MulBy " << fac << std::endl;
        }

        ~MulBy() noexcept { std::cerr << "~MulBy " << fac << std::endl; }

        MulBy(const MulBy &other) noexcept : fac(other.fac)
        {
            std::cerr << "+MulBy(@" <<  fac << ")" << std::endl;
        }

        inline double operator()(const double x) { return Run(x); }

        double Run(double x)
        {
            return fac * x;
        }

        int fac;

    private:
        Y_DISABLE_ASSIGN(MulBy);
    };
}

Y_UTEST(type_functor)
{

    {
        Functor<double,TL1(double)> Sin = cfunctor( Sin_ );
    }

    {
        MulBy obj = 4;

        Functor<double,TL1(double)> F1(&obj, & MulBy::Run );
        Functor<float,TL1(float)>   F2(obj);

        std::cerr << F1(2) << std::endl;
        std::cerr << F2(2) << std::endl;

        obj.fac = 6;
        std::cerr << F1(2) << std::endl;
        std::cerr << F2(2) << std::endl;

    }


}
Y_UDONE()

