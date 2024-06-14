
#include "y/system/progress.hpp"
#include "y/system/hrt.hpp"
#include <cstring>
#include <cmath>

namespace Yttrium
{
    const char Progress::Wheel[] =
    {
        '-', '\\', '|', '/'
    };

    Progress:: Progress() : eta(), width(32), cycle(0)
    {
        start();
    }

    Progress:: ~Progress() noexcept
    {
    }

    void Progress:: start() {
        eta.start();
    }

    void Progress:: finish(std::ostream &os)
    {
        const size_t one=1;
        show(os,eta(one,one));
        os << std::endl;
    }


    void Progress:: show(std::ostream &os, const double required)
    {
        const size_t numChars = static_cast<size_t>( floor(width*eta.fraction+0.5));
        const double percent  = floor(eta.fraction*1000+0.5)/10;
        ++cycle;
        cycle = cycle % Cycle;
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

}
