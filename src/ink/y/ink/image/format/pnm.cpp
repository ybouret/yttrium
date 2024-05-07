
#include "y/ink/image/format/pnm.hpp"
#include "y/string/boolean.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Ink
    {
        FormatPNM::  FormatPNM() : Format(CallSign,"(p[bgp]m)&") {}
        FormatPNM:: ~FormatPNM() noexcept {}

        const char * const FormatPNM:: CallSign = "PNM";

        static   bool QueryBinary(const FormatOptions * const options)
        {
            static const char    name[] = "binary";
            const String * const args = FormatOptions::Query(options,"binary");
            if(0==args) return false;
            return StringToBoolean::Get(*args,name);
        }

        FormatPNM:: Kind FormatPNM:: GetKind(const String &lowerCaseExt, const bool binary)
        {

            if("ppm" == lowerCaseExt) return binary ? P4 : P1;
            if("pgm" == lowerCaseExt) return binary ? P5 : P2;
            if("ppm" == lowerCaseExt) return binary ? P6 : P3;
            throw Specific::Exception(CallSign, "invalid extension '%s'", lowerCaseExt.c_str());
        }

        void  FormatPNM:: save(const Image         &image,
                               const String        &fileName,
                               const FormatOptions *options) const
        {
            const bool   binary = QueryBinary(options);
            const String ext    = LowerCaseExt(fileName); std::cerr << "ext=" << ext << " / binary=" << binary << std::endl;
            const Kind   kind   = GetKind(ext,binary);
        }

        Codec::Image FormatPNM:: load(const String        &fileName,
                                      const FormatOptions *options) const
        {
            return Image(1,1);
        }
    }
}

