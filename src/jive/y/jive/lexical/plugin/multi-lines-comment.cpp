#include "y/jive/lexical/plugin/multi-lines-comment.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {
            MultiLinesComment:: ~MultiLinesComment() noexcept
            {
            }

            Message MultiLinesComment:: leave(const Token &) noexcept
            {
                Y_XMLOG( getXMLog(), "<" << name << "> Enter");
                return LX_ENDL;
            }

            Message MultiLinesComment:: enter(const Token &) noexcept
            {
                Y_XMLOG( getXMLog(), "<" << name << "> Leave");
                return 0;
            }

            void MultiLinesComment:: setup()
            {
                endl("endl","[:endl:]");
                drop("comment",".+");
            }


        }

    }

}
