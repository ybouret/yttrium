
#include "y/ink/image/format/png.hpp"

namespace Yttrium
{
    namespace Ink
    {
        FormatPNG::  FormatPNG() : Format(CallSign,"(png)&") {}
        FormatPNG:: ~FormatPNG() noexcept {}

        const char * const FormatPNG:: CallSign = "PNG";

    }
}
