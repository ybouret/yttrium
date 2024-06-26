#include "y/system/sleep.hpp"
#include "y/utest/run.hpp"
#include "y/system/wtime.hpp"
#include "y/type/utils.hpp"
#include "y/system/progress.hpp"
#include "y/system/hrt.hpp"
#include <cstring>

using namespace Yttrium;


Y_UTEST(eta)
{
    WallTime tmx;
    const uint64_t mark = WallTime::Ticks();
    SystemSleep::For(20);
    const double   ell = tmx(WallTime::Ticks() - mark);
    std::cerr << "ell=" << ell*1000.0 << " ms" << std::endl;

    Y_USHOW(HRT::Minute);
    Y_USHOW(HRT::Hour);
    Y_USHOW(HRT::Day);
    Y_USHOW(HRT::MaxSeconds);



    for(double i=0;i<=HRT::MaxSeconds+10;++i)
    {
        const HRT hrt(i);
        Y_ASSERT(HRT::Length == strlen( *hrt ));
    }


#if 0
    {
        ETA eta;
        eta.start();
        const size_t nmax = 200;
        for(size_t i=1;i<=nmax;++i)
        {
            SystemSleep::For(30);
            const double t = eta(i,nmax);
            (std::cerr << "ETA : " << HRT(t) << " | runtime : " << HRT(eta.ellapsed) << '\r').flush();
        }
        std::cerr << std::endl;
    }
#endif


    {
        Progress bar;
        const size_t nmax = 34;
        for(size_t i=1;i<nmax;++i)
        {
            SystemSleep::For(75);
            bar(std::cerr,i,nmax);
        }
        bar.finish(std::cerr);
    }






}
Y_UDONE()

