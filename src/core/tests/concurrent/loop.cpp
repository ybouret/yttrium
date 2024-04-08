#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/thread.hpp"
#include "y/concurrent/split/trek.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include "y/string/env.hpp"

#include <cmath>

using namespace Yttrium;

namespace   {

    class Demo
    {
    public:
        explicit Demo() noexcept : count(10000000),  partial() {}
        virtual ~Demo() noexcept {}

        uint32_t       count;
        Vector<double> partial;

        inline void operator()(const Concurrent::ThreadContext &ctx)
        {
            assert(partial.size()>=ctx.size);
            const uint32_t offset = 1;
            const uint32_t length = count;
            const Concurrent::ForLoop<uint32_t> trek = Concurrent::Split::Using(ctx, length, offset);
            Y_ASSERT(Concurrent::ForLoopIncrease==trek.family);
            Y_THREAD_MSG("In " << ctx.name << ": from " << trek);
            double sum = 0;
            for(uint32_t i=trek.offset,j=trek.length;j>0;--j,++i)
            {
                sum += 1.0 / Squared( double(i) );
            }
            partial[ctx.indx] = sum;
        }

        inline double getFrom(const Concurrent::Loop &loop) const
        {
            double sum = 0;
            for(size_t i=1;i<=loop.size();++i)
                sum += partial[i];
            return sqrt(6.0*sum);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };

}

Y_UTEST(concurrent_loop)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    Demo                       demo;
    Concurrent::Mono           mono("mono");
    const Concurrent::Topology topo;
    Concurrent::Crew           crew(topo);
    WallTime                   tmx;

    Y_THREAD_MSG("now in main...");

    demo.partial.adjust(crew.size(),0);
    uint64_t mark = WallTime::Ticks();
    mono(demo);
    const double   sumMono = demo.getFrom(mono);
    const uint64_t tmxMono = WallTime::Ticks() - mark;

    mark = WallTime::Ticks();
    crew(demo);
    const double   sumCrew = demo.getFrom(crew);
    const uint64_t tmxCrew = WallTime::Ticks() - mark;

    std::cerr << "par: " << crew.size() << std::endl;
    std::cerr << "sum: " << std::setw(15) << sumMono << " / " << std::setw(15)  << sumCrew << std::endl;
    std::cerr << "tmx: " <<  std::setw(15) << HumanReadable(tmxMono) << " / " <<  std::setw(15) << HumanReadable(tmxCrew) << std::endl;
    const double speedUp = double(tmxMono)/double(tmxCrew);
    std::cerr << "spd: " << speedUp << std::endl;
}
Y_UDONE()

