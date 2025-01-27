
#include "y/apex/natural.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"
#include "y/apex/block/factory.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        size_t Natural:: serialize(OutputStream &fp) const
        {
            volatile AutoLock guard(*this);
            const Jig1 & jig = block->make<Plan1>();
            const size_t num = jig.words;
            const size_t hdr = fp.emitVBR(num);
            fp.frame(jig.word,num);
            return hdr+num;
        }

        Natural Natural:: Read(InputStream &fp)
        {
            static Factory & factory = Factory::Instance();
            const size_t     bytes   = fp.readVBR<size_t>("Apex::Natural bytes");
            Block * const    blk( factory.queryBytes(bytes) );
            Jig1 &           jig = blk->make<Plan1>();
            try {
                if(bytes!=fp.fetch(jig.word,bytes)) throw Specific::Exception(CallSign, "missing bytes in Read");
                blk->sync();
                if(bytes!=jig.words)                throw Specific::Exception(CallSign, "invalid bytes in Read");
            }
            catch(...)
            {
                factory.store(blk);
                throw;
            }
            return Natural(blk,AsBlock);
        }


    }

}
