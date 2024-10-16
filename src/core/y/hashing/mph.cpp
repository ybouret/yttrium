
#include "y/hashing/mph.hpp"
#include "y/data/list/cxx.hpp"
#include "y/object.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        class MinimalPerfect:: Node : public Object
        {
        public:
            static const int None = -1;
            typedef CxxListOf<Node> List;

            inline explicit Node() noexcept : code(None), chld(), next(0), prev(0)
            {
            }

            inline virtual ~Node() noexcept {}

            int   code;
            List  chld;
            Node *next;
            Node *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Node);
        };

        MinimalPerfect:: MinimalPerfect() : root( new Node() )
        {
        }

        MinimalPerfect:: ~MinimalPerfect() noexcept
        {
            assert(0!=root);
            Nullify(root);
        }
    }

}
