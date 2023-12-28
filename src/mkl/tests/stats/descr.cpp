#include "y/mkl/statistics/metrics.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/shuffle.hpp"
#include "../../../core/tests/main.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    static inline void doMetrics(Random::Bits &ran)
    {
        const size_t    n = 2 + ran.leq(20);
        Vector<T>       v(n,AsCapacity);
        List<T>         l(n,AsCapacity);

        for(size_t i=n;i>0;--i)
        {
            const T     X = Bring<T>::Get(ran);
            v << X;
            l << X;
        }
        std::cerr << v << std::endl;
        std::cerr << l << std::endl;

        Random::Shuffle::Range(v,ran);
        Random::Shuffle::Range(l,ran);
        std::cerr << v << std::endl;
        std::cerr << l << std::endl;

        Antelope::Add<T> xadd;

        const T vAverage = Statistics::Average(v,xadd);
        const T lAverage = Statistics::Average(l,xadd);
        std::cerr << "Average: " << vAverage << " | " << lAverage << std::endl;

        const T vVariance = Statistics::Variance(v,xadd,vAverage);
        const T lVariance = Statistics::Variance(l,xadd,lAverage);
        const T vStdDev0  = Sqrt<T>::Of(vVariance);
        const T lStdDev0  = Sqrt<T>::Of(lVariance);
        std::cerr << "StdDev0: " << vStdDev0 << " | " <<  lStdDev0 << std::endl;

        
        const T vStdDev = Statistics::StandardDeviation(v,xadd,vAverage);
        const T lStdDev = Statistics::StandardDeviation(l,xadd,lAverage);
        std::cerr << "StdDev : " << vStdDev << " | " <<  lStdDev << std::endl;

        const T vAbsDev = Statistics::AbsoluteDeviation(v,xadd,vAverage);
        const T lAbsDev = Statistics::AbsoluteDeviation(l,xadd,lAverage);
        std::cerr << "AbsDev : " << vAbsDev << " | " <<  lAbsDev << std::endl;

        const T vSkew = Statistics::Skewness(v, xadd, vAverage, vStdDev);
        const T lSkew = Statistics::Skewness(l, xadd, lAverage, lStdDev);
        std::cerr << "Skew   : " << vSkew << " | " <<  lSkew << std::endl;


        const T vKurt = Statistics::Kurtosis(v, xadd, vAverage, vStdDev);
        const T lKurt = Statistics::Kurtosis(l, xadd, lAverage, lStdDev);
        std::cerr << "Kurt   : " << vKurt << " | " <<  lKurt << std::endl;
    }
}


Y_UTEST(stats_descr)
{

    Random::Rand ran;

    doMetrics< XReal<long double> >(ran);
    doMetrics<float>(ran);
    doMetrics<long double>(ran);
    if(false) doMetrics<apq>(ran);

}
Y_UDONE()

