
#include "y/mkl/xreal.hpp"
#include "y/mkl/numeric.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/random/bits.hpp"
#include "y/sequence/vector.hpp"

#include <cmath>

using namespace Yttrium;
using namespace MKL;

template <typename T> static inline T genReal(Random::Bits &ran)
{
    const int    exponent = ran.in<int>(Numeric<T>::MIN_EXP+1,Numeric<T>::MAX_EXP-1);
    const T      mantissa = ran.in<T>(0.5,0.999);
    const T      r        = std::ldexp(mantissa,exponent);
    return ran.choice() ? -r : +r;

}
template <typename T>
static inline void checkXREAL(Random::Bits &ran)
{
    typedef XReal<T> xreal;
    typedef T        real;

    std::cerr << "Testing Multiplication" << std::endl;
    for(size_t i=0;i<8;++i)
    {
        real a = std::sqrt(std::abs(genReal<real>(ran)));
        real b = std::sqrt(std::abs(genReal<real>(ran)));
        if(ran.choice()) a=-a;
        if(ran.choice()) b=-b;
        const real c = a*b;
        const xreal A=a;
        const xreal B=b;
        const xreal C=A*B;
        std::cerr << c << " -> " << C << " -> " << T(C) << std::endl;
    }

    std::cerr << "Testing Division" << std::endl;
    for(size_t i=0;i<8;++i)
    {
        real a = std::sqrt(std::abs(genReal<real>(ran)));
        real b = std::sqrt(std::abs(genReal<real>(ran)));
        if(ran.choice()) a=-a;
        if(ran.choice()) b=-b;
        const real  c = a/b;
        const xreal A = a;
        const xreal B = b;
        const xreal C = A/B;
        std::cerr << a << "/" << b << " = " << A << "/" << B;
        std::cerr << " = " << c << " -> " << C << " -> " << T(C) << std::endl;
    }


    std::cerr << "Testing Addition" << std::endl;
    for(size_t i=0;i<8;++i)
    {
        real a =  genReal<real>(ran);
        real b =  genReal<real>(ran);
        real c = a+b;
        const xreal A = a;
        const xreal B = b;
        const xreal C = A + B;
        std::cerr << a << "+(" << b << ") = " << A << "+(" << B << ")";
        std::cerr << " = " << c << " -> " << C << " -> " << T(C) << std::endl;
    }

    std::cerr << "Testing Square Root" << std::endl;
    for(size_t i=0;i<8;++i)
    {
        real  a = std::fabs( genReal<real>(ran) );
        real  s = std::sqrt(a);
        xreal A = a;
        xreal S = A.sqrt();

        std::cerr << "sqrt(" << a << ") = " << s << std::endl;
        std::cerr << "sqrt(" << A << ") = " << S << " = " << T(S) << std::endl;

    }

    std::cerr << std::endl;
}

template <typename T>
static inline
T makeProd(const Readable<T> &seq)
{
    if(seq.size())
    {
        T res = seq[1];
        for(size_t i=2;i<=seq.size();++i)
        {
            res *= seq[i];
        }
        return res;
    }
    else
        return 0;
}

template <typename T>
static inline
T makeSum(const Readable<T> &seq)
{
    if(seq.size())
    {
        T res = seq[1];
        for(size_t i=2;i<=seq.size();++i)
        {
            res += seq[i];
        }
        return res;
    }
    else
        return 0;
}


template <typename T>
static inline void checkPow(const XReal<T> x, const T p)
{
    const XReal<T> y = x.pow(p);
    std::cerr << x << "^(" << p << ")=" << y << std::endl;
}

Y_UTEST(mkl_xreal)
{
    Random::Rand ran;
    checkXREAL<float>(ran);
    checkXREAL<double>(ran);
    checkXREAL<long double>(ran);

    Vector<double>          v;
    Vector< XReal<double> > V;
    for(int i=1;i<argc;++i)
    {
        const double d = atof(argv[i]);
        v << d;
        V << d;
    }
    std::cerr << "v=" << v << std::endl;
    std::cerr << "V=" << V << std::endl;

    {
        const double        p = makeProd(v);
        const XReal<double> P = makeProd(V);
        std::cerr << " prod = " << p << std::endl;
        std::cerr << "xprod = " << P << " = " << double(P) << std::endl;

    }

    {
        const double        s = makeSum(v);
        const XReal<double> S = makeSum(V);
        std::cerr << "  sum = " << s << std::endl;
        std::cerr << " xsum = " << S << " = " << double(S) << std::endl;

    }

    std::cerr << "Check Power" << std::endl;
    checkPow<double>(13,1.3);
    checkPow<float>(13,-1.3);
    checkPow<double>(13,0.6);
    checkPow<double>(13,-0.6);

    {
        const XReal<double> x(1,Raised,-1000); std::cerr << "x=" << x << std::endl;
        const XReal<double> y(7,Raised,-2000); std::cerr << "y=" << y << std::endl;
        const XReal<double> z = x*y;           std::cerr << "z=" << z << std::endl;
        Y_CHECK(z>0.0);
    }
}
Y_UDONE()

