

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

        }

    }

}

