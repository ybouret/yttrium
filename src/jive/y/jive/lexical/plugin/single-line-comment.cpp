
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            SingleLineComment:: ~SingleLineComment() noexcept
            {

            }

            Message SingleLineComment:: leave(const Token &) noexcept
            {
                Y_XMLOG( getXMLog(), "<" << name << "> Leave");
                return LX_ENDL;
            }

            Message SingleLineComment:: enter(const Token &) noexcept
            {
                Y_XMLOG( getXMLog(), "<" << name << "> Enter");
                return 0;
            }

            void SingleLineComment:: setup()
            {
                ret("[:endl:]");
                drop("comment",".+");
            }

            

        }

    }

}

