
#include "y/lingo/pattern.hpp"
#include <cstdlib>

namespace Yttrium
{
    namespace Lingo
    {
        Pattern:: ~Pattern() noexcept {}

        Pattern:: Pattern(const uint32_t t) noexcept :
        Quantized(), Serializable(), GraphViz::Vizible(),
        uuid(t),
        next(0),
        prev(0)
        {
        }

        Pattern:: Pattern(const Pattern &_) noexcept :
        Quantized(), Serializable(), GraphViz::Vizible(),
        uuid(_.uuid),
        next(0),
        prev(0)
        {
        }

        size_t Pattern:: emitUUID(OutputStream &fp) const
        {
            return fp.emitCBR(uuid);
        }

        bool Pattern:: multiple() const noexcept
        {
            return !univocal();
        }


        const char * Pattern:: ByteToRegExp(const uint8_t byte) noexcept
        {
            static const char *rx[256] = {
#               include "pattern/rx.hxx"
            };
            return rx[byte];
        }

    }

}


