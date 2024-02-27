#include "y/mkl/strain/filter.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/xreal.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"
#include "y/mkl/numeric.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/cxx-identifier.hpp"
#include "y/system/rtti.hpp"


using namespace Yttrium;
using namespace MKL;

template <typename T>
static inline void testStrain(const Readable<float> &x, const Readable<float> &y)
{
    static const String &tid = RTTI::Name<T>();
    const String         uid = CxxIdentifier::From(tid);
    std::cerr << uid << std::endl;
    Strain::Filter<T> f;

    for(size_t i=x.size();i>0;--i)
    {
        f.add(x[i],y[i]);
    }

    const String     fn = "strain-" + uid + ".dat";
    OutputFile fp(fn);

    for(float x0=-3.0f;x0<=3.0f;x0+=0.1f)
    {
        f.free();
        for(size_t i=x.size();i>0;--i)
        {
            if( fabsf( x[i]-x0 ) <= 1.0f )
            {
                f.add(x[i],y[i]);
            }
        }
        fp("%.15g",x0);
        for(unsigned degree=0;degree<=4;++degree)
        {
            f.eval(x0,degree);
            fp(" %.15g",float(f[1]));
        }
        fp << '\n';
    }



}


Y_UTEST(strain_filter)
{
    Random::Rand  ran;
    const size_t  np = 100 + ran.leq(100);
    Vector<float> x(np,AsCapacity);
    Vector<float> y(np,AsCapacity);

    for(size_t i=0;i<np;++i)
    {
        x << ran.symm<float>() * Numeric<float>::PI;
        y << 0.1f + 1.3f * sinf( x.tail() ) + 0.1f * ran.symm<float>();;
    }

    HeapSort::Call(x, Comparison::Increasing<float>, y);

    {
        OutputFile fp("strain-data.dat");
        for(size_t i=1;i<=np;++i) fp("%.15g %.15g\n",x[i],y[i]);
    }



    testStrain<float>(x,y);
    testStrain<double>(x,y);
    testStrain<long double>(x,y);
    testStrain< XReal<float> >(x,y);
    testStrain< XReal<double> >(x,y);
    testStrain< XReal<long double> >(x,y);

}
Y_UDONE()

