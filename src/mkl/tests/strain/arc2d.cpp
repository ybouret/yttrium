
#include "y/mkl/strain/arc2d.hpp"
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

template <typename T> static inline
void testArc2D(const Readable<float>        &theta,
               const Readable< V2D<float> > &r)
{
    static const String &tid = RTTI::Name<T>();
    const String         uid = CxxIdentifier::From(tid);
    const String         fn = "arc2d-" + uid + ".dat";
    std::cerr << uid << std::endl;
    Strain::Arc2D<T> af;


    const T scale(0.5);

    OutputFile fp(fn);
    for(float t=-3.0f; t<= 3.0f; t += 0.1f)
    {
        af.free();
        for(size_t i=theta.size();i>0;--i)
        {
            if( fabsf(t-theta[i]) <= 1.0f )
                af.add(theta[i],r[i]);
        }
        af.eval(t, 3, 3);
        const V2D<T> &pos = af.r;
        const V2D<T> &spd = af.v;
        const V2D<T>  rot = spd.ortho() / af.velocity;
        const V2D<T>  inn = pos + scale * af.CartesianCurvature() * rot;
        fp("%.15g %.15g %.15g %.15g %.15g\n", t, double(pos.x), double(pos.y), double(inn.x), double(inn.y) );
    }

}


Y_UTEST(strain_arc2d)
{
    Random::Rand  ran;

    Vector< float >      theta;
    Vector< V2D<float> > r;

    for(size_t i=100;i>0;--i)
    {
        theta << ran.symm<float>() * Numeric<float>::PI;
        const float c = cosf(theta.tail());
        const float s = sinf(theta.tail());
        r << V2D<float>( 0.7f + 1.2f * c + 0.05f * ran.symm<float>(),
                        -0.3f + 0.8f * s + 0.05f * ran.symm<float>());
    }

    HeapSort::Call(theta, Comparison::Increasing<float>, r);

    {
        OutputFile fp("arc2d.dat");
        for(size_t i=1;i<=theta.size();++i)
        {
            fp("%.15g %.15g %.15g\n", theta[i], r[i].x, r[i].y);
        }
    }


    testArc2D<float>(theta,r);
    testArc2D<double>(theta,r);
    testArc2D<long double>(theta,r);

    testArc2D< XReal<float> >(theta,r);
    testArc2D< XReal<double> >(theta,r);
    testArc2D< XReal<long double> >(theta,r);


}
Y_UDONE()
