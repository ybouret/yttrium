
#include "y/lingo/syntax/internal/wildcard.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            Wildcard:: ~Wildcard() noexcept
            {

            }

            


            void Wildcard:: vizLink(OutputStream &fp) const
            {
                vizArrow(fp,rule);
            }

        }

    }

}


