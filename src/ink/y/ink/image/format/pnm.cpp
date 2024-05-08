
#include "y/ink/image/format/pnm.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Ink
    {

     

        FormatPNM::  FormatPNM() : Format(CallSign,"(p[bgp]m)&") {}
        FormatPNM:: ~FormatPNM() noexcept {}

        const char * const FormatPNM:: CallSign = "PNM";

      

        FormatPNM:: Kind FormatPNM:: GetKind(const String &lowerCaseExt, const bool binary)
        {
            if("pbm" == lowerCaseExt) return binary ? P4 : P1;
            if("pgm" == lowerCaseExt) return binary ? P5 : P2;
            if("ppm" == lowerCaseExt) return binary ? P6 : P3;
            throw Specific::Exception(CallSign, "invalid extension '%s'", lowerCaseExt.c_str());
        }




    }
}

