
#include "y/lingo/pattern.hpp"

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


    }

}


