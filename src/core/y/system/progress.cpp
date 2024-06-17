
#include "y/system/progress.hpp"
#include "y/system/hrt.hpp"
#include <cstring>
#include <cmath>
#include <cstdio>

namespace Yttrium
{
    const char Progress::Wheel[] =
    {
        '-', '\\', '|', '/'
    };

    Progress:: Progress() : eta(), width(32), cycle(0), ended(false)
    {
        start();
    }

    Progress:: ~Progress() noexcept
    {
    }

    void Progress:: start() {
        eta.start();
        ended = false;
    }

    void Progress:: finish(std::ostream &os)
    {
        if(!ended)
        {
            const size_t one=1;
            (*this)(os,one,one);
            assert(true==ended);
        }
        os << std::endl;
    }


    void Progress:: show(std::ostream &os, const double required)
    {
        const size_t numChars = static_cast<size_t>( floor(width*eta.fraction+0.5));
        const double percent  = floor(eta.fraction*1000+0.5)/10;

        if(ended)
        {
            os << "[*]";
        }
        else
        {
            ++cycle;
            cycle = cycle % Cycle;
            os << '[' << Wheel[cycle] << ']';
        }
        
        os << '[';
        for(size_t i=0;i<numChars;++i)     os << '#';
        for(size_t i=numChars;i<width;++i) os << ' ';
        os << ']';

        char buffer[16];
        memset(buffer,0,sizeof(buffer));
        snprintf(buffer,sizeof(buffer),"[%5.1f%%]",percent);
        os << buffer;

        if(ended)
        {
            const HRT    run_time = eta.ellapsed;
            os << " RUN " << run_time;

        }
        else
        {
            const HRT    awaiting = required;
            os << " ETA " << awaiting;
        }


        os << '\r';

        os.flush();
    }

}
