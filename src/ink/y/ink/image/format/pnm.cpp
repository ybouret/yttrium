
#include "y/ink/image/format/pnm.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    namespace Ink
    {
        FormatPNM::  FormatPNM() : Format(CallSign,"(p[bgp]m)&") {}
        FormatPNM:: ~FormatPNM() noexcept {}

        const char * const FormatPNM:: CallSign = "PNM";

        static   bool QueryBinary(const FormatOptions * const options)
        {
            const String *bin = FormatOptions::Query(options,"binary");
            if(0==bin) return false;
            String res = *bin;
            TextOps::ToLower(&res[1], res.size());
            if( "1" == res || "on" == res || "true" == res) return true;

            return false;
        }

        void  FormatPNM:: save(const Image         &image,
                               const String        &fileName,
                               const FormatOptions *options) const
        {

        }

    }
}

