

#include "y/mkl/root/zbis.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/xreal.hpp"
#include "y/system/rtti.hpp"
#include <cmath>

using namespace Yttrium;
using namespace MKL;



namespace
{
    template <typename T>
    struct callF
    {
        size_t calls;

        inline T operator()(const T x)
        {
            ++calls;
            return x*x*x-T(1.860867);
        }
    };

    template <typename T>
    static inline
    void doZFind()
    {
        std::cerr << std::endl <<  "ZFind<" << RTTI::Name<T>() << ">" << std::endl;
        callF<T>   F = { 0 };
        Triplet<T> x = { -1, 0, 2};
        Triplet<T> f = { F(x.a), 0, F(x.c) };
        std::cerr << x << " -> " << f << std::endl;

        ZBis<T> zbis;
        zbis(F, x, f);
        std::cerr << "f(" << x.b << ")=" << f.b << " in #call=" << F.calls << std::endl;

    }

}


Y_UTEST(root_zfind)
{
    doZFind<float>();
    doZFind<double>();
    doZFind<long double>();
    doZFind< XReal<float> >();
    doZFind< XReal<double> >();
    doZFind< XReal<long double> >();
    
}
Y_UDONE()
