
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

        String Pattern:: ToRegExp(const String &s)
        {
            const size_t n = s.size();
            String res(4*n,AsCapacity,false);
            for(size_t i=1;i<=n;++i)
            {
                char          buff[8] = {0,0,0,0,0,0,0,0};
                const uint8_t byte    = s[i];
                snprintf(buff,sizeof(buff)-1,"\\x%02x",byte);
                res += buff;
            }
            return res;
        }

    }

}

