

#include "y/lingo/syntax/internal/wildcard/repeat.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Repeat:: ~Repeat() noexcept
            {
            }

            Caption Repeat:: NameFor(const Rule &r, const size_t n)
            {
                String s = *r.name;
                switch(n)
                {
                    case 0: s += '*'; break;
                    case 1: s += '+'; break;
                    default:
                        s += Formatted::Get(">=%u", unsigned(n));
                }
                return Caption(s);
            }
        }

    }

}
