#include "y/jive/pattern/basic.hpp"
#include "y/io/stream/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Pattern * Pattern:: ReadFrom(InputStream &fp)
        {
            const uint32_t uuid = fp.readCBR<uint32_t>("Pattern.UUID");

            switch(uuid)
            {
                case Single::UUID: return new Single( fp.readCBR<uint8_t>("Single.code") );
                case Range:: UUID: {
                    const uint8_t lower = fp.readCBR<uint8_t>("Range.lower");
                    const uint8_t upper = fp.readCBR<uint8_t>("Range.upper");
                    return new Range(lower,upper);
                }
                default:
                    break;
            }
            throw Specific::Exception("Pattern::ReadFrom(InputStream)","%s %s",Core::Unknown,FourCC::ToText(uuid));
        }

    }

}
