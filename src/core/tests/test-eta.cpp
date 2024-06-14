#include "y/system/sleep.hpp"
#include "y/utest/run.hpp"
#include "y/system/wtime.hpp"
#include "y/type/utils.hpp"
#include "y/system/eta.hpp"
#include "y/system/hrt.hpp"
#include <cstring>

using namespace Yttrium;

namespace Yttrium
{

    class Progress
    {
    public:
        static const char   Wheel[4];
        static const size_t Cycle = sizeof(Wheel)/sizeof(Wheel[0]);

        explicit Progress() : eta(), width(32), cycle(0)
        {
            init();
        }

        virtual ~Progress() {}

        ETA    eta;
        size_t width;
        size_t cycle;

        void init() {
            eta.start();
        }

        template <typename T> inline
        void display(std::ostream &os, const T &istep, const T &total)
        {
            show(os,eta(istep,total));
        }

        void finish(std::ostream &os)
        {
            const size_t one=1;
            show(os,eta(one,one));
            os << std::endl;
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Progress);
        void show(std::ostream &os, const double required)
        {
            const size_t numChars = static_cast<size_t>( floor(width*eta.fraction+0.5));
            const double percent  = floor(eta.fraction*1000+0.5)/10;
            cycle = ++cycle % Cycle;
            os << '[' << Wheel[cycle] << ']';
            os << '[';
            for(size_t i=0;i<numChars;++i)     os << '#';
            for(size_t i=numChars;i<width;++i) os << ' ';
            os << ']';

            char buffer[16];
            memset(buffer,0,sizeof(buffer));
            snprintf(buffer,sizeof(buffer),"[%5.1f%%]",percent);
            os << buffer;

            if(eta.fraction<1.0)
            {
                const HRT    awaiting = required;
                os << " ETA " << awaiting;
            }
            else
            {
                const HRT    run_time = eta.ellapsed;
                os << " RUN " << run_time;

            }


            os << '\r';

            os.flush();
        }

    };

    const char Progress::Wheel[] =
    {
        '-', '\\', '|', '/'
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
            bar.display(std::cerr,i,nmax);
        }
        bar.finish(std::cerr);
    }






}
Y_UDONE()

