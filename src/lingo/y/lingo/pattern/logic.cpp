
#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Logic:: ~Logic() noexcept
        {
        }

        Logic:: Logic(const uint32_t t) noexcept :
        Pattern(t),
        Pattern::List()
        {
        }

        Logic:: Logic(const Logic &logic) :
        Pattern(logic),
        Pattern::List(logic)
        {
        }
        
        size_t Logic:: serialize(OutputStream &fp) const
        {
            size_t ans = emitUUID(fp);
            ans += fp.emitVBR(size);
            for(const Pattern *p=head;p;p=p->next)
                ans += p->serialize(fp);
            return ans;
        }


    }

}

