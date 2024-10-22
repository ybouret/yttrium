
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

        Logic & Logic:: operator<<(Pattern * const p) noexcept
        {
            assert(0!=p);
            pushTail(p);
            return *this;
        }

        void Logic:: vizLink(OutputStream &fp) const
        {
            for(const Pattern *node=head;node;node=node->next)
            {
                Endl(Arrow(fp,this,node));
            }
        }
    }

}


#include "y/lingo/pattern/basic/byte.hpp"
#include "y/lingo/pattern/basic/range.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Logic & Logic:: add(const uint8_t single)
        {
            pushTail( new Byte(single) );
            return *this;
        }

        Logic & Logic:: add(const uint8_t lower, const uint8_t upper)
        {
            pushTail( new Range(lower,upper) );
            return *this;
        }
    }

}


