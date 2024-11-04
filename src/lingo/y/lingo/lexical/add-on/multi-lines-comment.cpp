

#include "y/lingo/lexical/add-on/multi-lines-comment.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            
            MultiLinesComment:: ~MultiLinesComment() noexcept
            {
            }

            void MultiLinesComment:: setup()
            {
                drop("[:dot:]");
                endl("[:endl:]",Unit::Drop);
            }

            void MultiLinesComment:: enter(const Token &)
            {
            }

            void MultiLinesComment:: leave(const Token &)
            {
            }
        }

    }

}

