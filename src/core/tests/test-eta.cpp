#include "y/system/sleep.hpp"
#include "y/utest/run.hpp"
#include "y/system/wtime.hpp"
#include "y/type/utils.hpp"
#include "y/system/hrt.hpp"
#include <cstring>

using namespace Yttrium;

namespace Yttrium
{
    class ETA
    {
    public:

        explicit ETA() :
        wallTime(),
        iniTicks(0),
        nowTicks(0),
        procTime(0)
        {
        }

        virtual ~ETA() noexcept {}

        void start() {
            Coerce(iniTicks) = WallTime::LockedTicks();
            Coerce(nowTicks) = iniTicks;
            Coerce(procTime) = 0;
        }

        template <typename T>
        inline double operator()(const T &istep,
                                 const T &total)
        {
            const long double ella = Coerce(procTime) =wallTime( (Coerce(nowTicks) = WallTime::LockedTicks() ) - iniTicks );
            const long double done = static_cast<long double>(istep) / total;
            const long double todo = 1.0l - done;
            const long double num  = todo * ella;
            if( num >= done * HRT::MaxSeconds )
            {
                return HRT::MaxSeconds;
            }
            else
            {
                return double(num/done);
            }
        }

        WallTime             wallTime;
        const uint64_t       iniTicks;
        const uint64_t       nowTicks;
        const double         procTime;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ETA);
    };
}

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


    ETA eta;
    eta.start();
    const size_t nmax = 100;
    for(size_t i=1;i<=nmax;++i)
    {
        SystemSleep::For(30);
        const double t = eta(i,nmax);
        (std::cerr << "ETA : " << HRT(t) << " | runtime : " << HRT(eta.procTime) << '\r').flush();
    }
    std::cerr << std::endl;






}
Y_UDONE()

