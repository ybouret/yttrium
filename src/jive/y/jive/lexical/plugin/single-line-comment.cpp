
#include "y/jive/lexical/plugin/single-line-comment.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            SingleLineComment:: ~SingleLineComment() noexcept
            {

            }

            void SingleLineComment:: setup()
            {
                Scanner &self = *this;
                back("[:endl:]", parent, self, & Scanner::newLineAndDrop);
                self("any",".+",false);
            }

        }

    }

}

