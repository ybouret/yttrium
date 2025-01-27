
#include "y/apex/natural.hpp"
#include "y/stream/output.hpp"

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

    }

}
