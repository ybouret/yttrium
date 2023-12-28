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
        const size_t    n = 1 + ran.leq(20);
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
        std::cerr << vAverage << "/" << lAverage << std::endl;

    }
}


Y_UTEST(stats_descr)
{

    Random::Rand ran;

    doMetrics<float>(ran);
    doMetrics<apq>(ran);
    doMetrics< XReal<long double> >(ran);
}
Y_UDONE()

