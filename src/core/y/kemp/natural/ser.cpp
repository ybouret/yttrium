#include "y/kemp/natural.hpp"
#include "y/kemp/element.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"
#include "y/ptr/auto.hpp"
#include "y/stream/io/variable-info.hpp"

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


        Natural Natural:: ReadFrom(InputStream &fp, const char * name)
        {
            if(!name) name = CallSign;
            IO::VariableInfo<256> info;
            const size_t          size = fp.readVBR<size_t>( info("%s.bytes",name) );
            AutoPtr<Element>      elem = new Element(size,AsCapacity);
            Assembly<uint8_t> &   data = elem->get<uint8_t>();
            fp.fetch(data.item, data.positive = size );
            elem->bits = data.updateBits();
            return Natural( elem.yield()->revise(), AsElement);
        }
    }

}

