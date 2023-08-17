
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

        bool operator==(const Pattern &lhs, const Pattern &rhs) noexcept
        {
            return lhs.isEqualTo(rhs);
        }

        bool operator!=(const Pattern &lhs, const Pattern &rhs) noexcept
        {
            return !lhs.isEqualTo(rhs);
        }

        size_t Pattern:: save(const String &datName) const
        {
            Libc::OutputFile fp(datName);
            return serialize(fp);
        }

        size_t Pattern:: save(const char *datName) const
        {
            const String _(datName); return save(_);
        }

        void Pattern:: graphViz(const String &dotName) const
        {
            Libc::OutputFile fp(dotName);
            Vizible::Enter(fp,"G");
            viz(fp);
            Vizible::Leave(fp);
        }
    }

}

