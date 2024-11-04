
#include "y/lingo/lexical/add-on/single-line-comment.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            const char * const SingleLineComment::EndlExpr = "[:endl:]";

            SingleLineComment:: ~SingleLineComment() noexcept
            {
            }

            void SingleLineComment:: setup()
            {
                drop("[:dot:]");
            }

        }

    }

}

