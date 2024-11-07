

#include "y/lingo/syntax/internal.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Internal:: ~Internal() noexcept {}
            

        }
    }
}


namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {


            XNode:: XNode(const Syntax::Internal & _rule) noexcept:
            Object(),
            rule(_rule),
            type(Internal),
            sire(0),
            next(0),
            prev(0)
            {
                assert(Internal==rule.typeOfNode());
                zero();
                new( & list() ) XList();
            }
        }

    }

}

