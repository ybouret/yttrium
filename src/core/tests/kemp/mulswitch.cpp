
#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include <cstring>

#include "y/text/ascii/convert.hpp"
#include "y/stream/libc/output.hpp"
#include "y/text/human-readable.hpp"

using namespace Yttrium;
using namespace Kemp;



class Sampler
{
public:
    static const int width = 6;

    explicit Sampler(const double dd,
                     const Ops    how) :
    chrono(),
    tmxStd(0),
    tmxFFT(0),
    cycles(0),
    ran(),
    rtm(dd),
    ops(how)
    {

    }

    ~Sampler() noexcept
    {
    }

    inline double rateStd()
    {
        return cycles/ chrono(tmxStd);
    }

    inline double rateFFT()
    {
        return cycles/ chrono(tmxFFT);
    }

    inline bool operator()(const size_t lhs,
                           const size_t rhs)
    {
        tmxStd = 0;
        tmxFFT = 0;
        cycles = 0;
        do {
            update(ops, lhs, rhs);
        } while( chrono(tmxFFT) < rtm);

        const uint64_t srate = static_cast<uint64_t>( rateStd() );
        const uint64_t frate = static_cast<uint64_t>( rateFFT() );
        const bool     res   = tmxFFT <= tmxStd;
        std::cerr
        << "(" << std::setw(width) << lhs
        << ","
        << std::setw(width) << rhs << ") => "
        << HumanReadable(srate)
        << " / "
        << HumanReadable(frate);

        if(res) std::cerr << " (*)";

        std::cerr << std::endl;

        return res;
    }

    inline bool operator()(const size_t n)
    {
        return (*this)(n,n);
    }


    WallTime           chrono;
    uint64_t           tmxStd;
    uint64_t           tmxFFT;
    size_t             cycles;
    Random::ParkMiller ran;
    double             rtm;
    Ops                ops;

private:
    Y_DISABLE_COPY_AND_ASSIGN(Sampler);

    inline void update(const Ops    ops,
                       const size_t lhs,
                       const size_t rhs)
    {
        ++cycles;

        Element l(lhs,ran);
        Element r(rhs,ran);

        {
            const uint64_t   mark = WallTime::Ticks();
            AutoPtr<Element> prod = Element::Mul[ops](l,r);
            tmxStd += WallTime::Ticks() - mark;
        }

        {
            const uint64_t   mark = WallTime::Ticks();
            AutoPtr<Element> prod = Element::MulFFT(l,r);
            tmxFFT += WallTime::Ticks() - mark;
        }
    }

};







Y_UTEST(kemp_mulswitch)
{
    double  run = 0.1;
    Ops     ops = Ops64_32;

    if(argc>1)
    {
        run = ASCII::Convert::ToReal<double>(argv[1], "run");
    }

    Sampler perf(run,ops);

    std::cerr << "Look Up" << std::endl;
    unsigned p = 0;
    while(true)
    {
        const size_t n = 1 << p;
        if(perf(n)) break;
        ++p;
    }

    std::cerr << "Bissect" << std::endl;
    size_t upper = (1<<p);
    size_t lower = upper/ 2;

    Y_ASSERT(perf(upper,upper));
    Y_ASSERT(!perf(lower,lower));
    while(upper-lower>1)
    {
        const size_t mid = (upper+lower)>>1;
        if(perf(mid))
        {
            upper = mid;
        }
        else
        {
            lower = mid;
        }
    }
    std::cerr << "upper sym bits=" << upper << std::endl;
    upper = Y_ALIGN8(upper);
    std::cerr << "upper sym bits=" << upper << std::endl;

    OutputFile fp("switch.dat");
    for(int i=-64; i <= 64; i += 8)
    {
        for(int j=-64; j <= 64; j += 8)
        {
            (void) perf(upper+i,upper+j);
            fp("%d %d %g\n",i,j, perf.rateFFT() - perf.rateStd() );
        }
        fp << '\n';
    }





}
Y_UDONE()


