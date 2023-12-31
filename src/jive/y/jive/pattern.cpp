
#include "y/jive/pattern.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Pattern:: ~Pattern() noexcept
        {
        }

        Pattern:: Pattern(const uint32_t t) noexcept :
        Object(),
        Counted(),
        Token(),
        Serializable(),
        Vizible(),
        next(0),
        prev(0),
        self(0),
        uuid(t)
        {
        }

        Pattern:: Pattern(const Pattern &p) noexcept :
        Object(),
        Counted(),
        Token(),
        Serializable(),
        Vizible(),
        next(0),
        prev(0),
        self(0),
        uuid(p.uuid)
        {

        }

        bool Pattern:: isRegular() const noexcept
        {
            return ! isFragile();
        }

        bool Pattern:: isMultiple() const noexcept
        {
            return ! isUnivocal();
        }



        bool operator==(const Pattern &lhs, const Pattern &rhs) noexcept
        {
            return lhs.isEqualTo(rhs);
        }

        bool operator!=(const Pattern &lhs, const Pattern &rhs) noexcept
        {
            return !lhs.isEqualTo(rhs);
        }

        void Pattern:: graphViz(OutputStream &fp) const
        {
            Vizible::Enter(fp, "G");
            viz(fp);
            Vizible::Leave(fp);
        }
        
    }

}

