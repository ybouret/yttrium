

#include "y/jive/pattern/basic/any1.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"


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


    }

}
