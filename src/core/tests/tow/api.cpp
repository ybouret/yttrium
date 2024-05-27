#include "y/tow/api.hpp"
#include "y/utest/run.hpp"
#include <cstring>
#include "y/utest/timing.hpp"
#include "y/random/park-miller.hpp"
#include "y/system/rtti.hpp"
#include "y/random/fill.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

template <typename TARGET, typename SOURCE>
static inline void Perform(TARGET * const target,SOURCE * const source, Random::Bits &ran)
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

    std::cerr << sname << " => " << tname << std::endl;
    Random::Fill::Block(source, sourceCount*sizeof(SOURCE), ran);
    Hexadecimal::Display(std::cerr << "\tsource=", source, sourceCount) << std::endl;

    TOW::Transmute(target,source,targetCount);
    Hexadecimal::Display(std::cerr << "\ttarget=", target, targetCount) << std::endl;

}


Y_UTEST(tow_api)
{
    Random::ParkMiller ran;

    uint8_t   a8[8]; Y_STATIC_ZARR(a8);
    uint16_t a16[4]; Y_STATIC_ZARR(a16);
    uint32_t a32[2]; Y_STATIC_ZARR(a32);
    uint64_t a64[1]; Y_STATIC_ZARR(a64);


    std::cerr << "Source=a8" << std::endl;
    Perform( a8,a8,ran);
    Perform(a16,a8,ran);
    Perform(a32,a8,ran);
    Perform(a64,a8,ran);
    std::cerr << std::endl;

    std::cerr << "Source=a16" << std::endl;
    Perform( a8,a16,ran);
    Perform(a16,a16,ran);
    Perform(a32,a16,ran);
    Perform(a64,a16,ran);
    std::cerr << std::endl;

}
Y_UDONE()
