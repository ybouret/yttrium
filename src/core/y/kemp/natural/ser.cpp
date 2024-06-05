#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        size_t Natural:: serialize(OutputStream &fp) const
        {
            assert(0!=code);
            const Assembly<uint8_t> &data = code->get<uint8_t>();
            const size_t             toWrite = data.positive;
            size_t nw = fp.emitVBR(toWrite);
            fp.frame(data.item,toWrite);
            return nw + toWrite;
        }

    }

}

