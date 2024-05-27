#include "y/tow/api.hpp"
#include "y/utest/run.hpp"
#include <cstring>
#include "y/utest/timing.hpp"
#include "y/random/park-miller.hpp"
#include "y/system/rtti.hpp"
#include "y/random/fill.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/type/utils.hpp"

using namespace Yttrium;
static uint64_t rate[4][4];

template <typename TARGET, typename SOURCE>
static inline uint64_t Perform(TARGET * const target,SOURCE * const source,
                               Random::Bits &ran,
                               Timing       &tmx)
{
    const String &tname = RTTI::Name<TARGET>();
    const String &sname = RTTI::Name<SOURCE>();

    size_t sourceCount = 0;
    size_t targetCount = 0;
    switch( TOW::API<TARGET,SOURCE>::Action )
    {
        case TOW::Collect: targetCount = 1; sourceCount = sizeof(TARGET)/sizeof(SOURCE); break;
        case TOW::RawCopy: sourceCount = targetCount = 1; break;
        case TOW::Scatter: targetCount = sizeof(SOURCE)/sizeof(TARGET); sourceCount = 1; break;
    }
    const size_t sourceBytes = sourceCount * sizeof(SOURCE);
    const size_t cycles      = Min(sourceCount,targetCount);
    Y_ASSERT(1==cycles);

    std::cerr << sourceCount << "*" << sname << " => " << targetCount << "*" << tname << std::endl;
    Random::Fill::Block(source, sourceBytes, ran);
    Hexadecimal::Display(std::cerr << "\tsource=", source, sourceCount) << std::endl;

    TOW::Transmute(target,source,cycles);
    Hexadecimal::Display(std::cerr << "\ttarget=", target, targetCount) << std::endl;


    tmx.reset();

    do
    {
        Random::Fill::Block(source, sourceBytes, ran);
        const uint64_t mark = WallTime::Ticks();
        TOW::Transmute(target,source,cycles);
        tmx.renew(mark);
    } while(tmx.probe()<0.1);

    const uint64_t rate = tmx.speed();
    std::cerr << "\trate  = " << HumanReadable(rate) << " ops/s" << std::endl;

    return rate;
}


Y_UTEST(tow_api)
{
    Random::ParkMiller ran;
    Timing             tmx;
    memset(&rate[0][0],0,sizeof(rate));

    uint8_t   a8[8]; Y_STATIC_ZARR(a8);
    uint16_t a16[4]; Y_STATIC_ZARR(a16);
    uint32_t a32[2]; Y_STATIC_ZARR(a32);
    uint64_t a64[1]; Y_STATIC_ZARR(a64);


    std::cerr << "Source=a8" << std::endl;
    rate[0][0] = Perform( a8,a8,ran,tmx);
    rate[1][0] = Perform(a16,a8,ran,tmx);
    rate[2][0] = Perform(a32,a8,ran,tmx);
    rate[3][0] = Perform(a64,a8,ran,tmx);
    std::cerr << std::endl;

    std::cerr << "Source=a16" << std::endl;
    rate[0][1] =  Perform( a8,a16,ran,tmx);
    rate[1][1] =  Perform(a16,a16,ran,tmx);
    rate[2][1] =  Perform(a32,a16,ran,tmx);
    rate[3][1] =  Perform(a64,a16,ran,tmx);
    std::cerr << std::endl;


    std::cerr << "Source=a32" << std::endl;
    rate[0][2] =  Perform( a8,a32,ran,tmx);
    rate[1][2] =  Perform(a16,a32,ran,tmx);
    rate[2][2] =  Perform(a32,a32,ran,tmx);
    rate[3][2] =  Perform(a64,a32,ran,tmx);
    std::cerr << std::endl;


    std::cerr << "Source=a64" << std::endl;
    rate[0][3] =  Perform( a8,a64,ran,tmx);
    rate[1][3] =  Perform(a16,a64,ran,tmx);
    rate[2][3] =  Perform(a32,a64,ran,tmx);
    rate[3][3] =  Perform(a64,a64,ran,tmx);
    std::cerr << std::endl;

    for(unsigned t=0;t<4;++t)
    {
        std::cerr << "target=" << std::setw(2) << (1<<t)*8 << "bits";
        for(unsigned s=0;s<4;++s)
        {
            std::cerr << " | " <<  HumanReadable(rate[t][s]);
        }
        std::cerr << std::endl;
    }

}
Y_UDONE()
