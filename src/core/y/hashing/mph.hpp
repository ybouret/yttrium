//! file


#ifndef Y_Hashing_Minimal_Perfect_Included
#define Y_Hashing_Minimal_Perfect_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        class MinimalPerfect
        {
        public:
            explicit MinimalPerfect();
            virtual ~MinimalPerfect() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MinimalPerfect);
            class Node;
            Node *root;
        };
    }
}

#endif

