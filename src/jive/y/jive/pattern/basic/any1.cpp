

#include "y/jive/pattern/basic/any1.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/stream/output.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Any1:: ~Any1() noexcept
        {
        }


        Any1:: Any1() noexcept :
        Pattern(UUID)
        {
            Y_PATTERN(Any1);
        }

        Pattern * Any1:: clone() const
        {
            return new Any1();
        }

        bool Any1:: takes(Source &source)
        {
            assert(0==size);
            Char *ch = source.get();
            if(!ch)
            {
                return false;
            }
            else
            {
                pushTail(ch);
                return true;
            }
        }

        void Any1::query(FirstChars &fc) const
        {
            fc.includeAll();
        }

        size_t Any1:: serialize(OutputStream &fp) const
        {
            return fp.emitCBR(uuid);
        }

        void Any1:: reset() noexcept { release(); }

        bool Any1:: isFragile() const noexcept
        {
            return false;
        }

        bool Any1:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid;
        }

        void Any1:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp, "any1") << ",shape=circle";
            fp << ']';
            Endl(fp);
        }

        bool    Any1::  isUnivocal() const noexcept
        {
            return false;
        }
    }

}
