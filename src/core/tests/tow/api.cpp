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

    std::cerr << sname << " => " << tname << std::endl;
    Random::Fill::Fuzzy(*source,ran);
    Hexadecimal::Display(std::cerr << "source=", source,1) << std::endl;
    
    TOW::Transmute(target,source,1);
}


Y_UTEST(tow_api)
{
    Random::ParkMiller ran;

    uint8_t   a8[8]; Y_STATIC_ZARR(a8);
    uint16_t a16[4]; Y_STATIC_ZARR(a16);
    uint32_t a32[2]; Y_STATIC_ZARR(a32);
    uint64_t a64[1]; Y_STATIC_ZARR(a64);


    // collect
    Perform(a16,a8,ran);


}
Y_UDONE()
