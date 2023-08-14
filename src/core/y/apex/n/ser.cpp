
#include "y/apex/natural.hpp"
#include "y/apex/n/cxx.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"
#include "y/memory/buffer/of.hpp"

namespace Yttrium
{

    namespace Apex
    {
        size_t Natural:: serialize(OutputStream &fp) const
        {
            const Prototype &self    = CONST_PROTO(*this);
            const size_t     size    = self.bytes;
            size_t           written = fp.emitVBR(size);
            for(size_t i=0;i<size;++i,++written)
                fp.write(self.getByte(i));
            return written;
        }

        Natural Natural:: ReadFrom(InputStream &fp)
        {
            Natural      res;
            const size_t size = fp.readVBR<size_t>("apn.size");
            if(size>0)
            {
                Memory::BufferOf<uint8_t> buff(size);
                for(size_t i=0;i<size;++i)
                {
                    buff[i] = fp.readCBR<uint8_t>("apn.byte");
                }
                //buff.displayHexadecimal(std::cerr << "buff=") << std::endl;
                size_t j=size;
                res = buff[--j];
                while(j>0)
                {
                    res *= 256;
                    res += buff[--j];
                }
            }
            return res;
        }
    }

}


