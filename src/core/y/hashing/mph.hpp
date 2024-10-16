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

            MinimalPerfect & operator()(const void * const data,
                                        size_t             size,
                                        const int          hash);

            MinimalPerfect & operator()(const Memory::ReadOnlyBuffer &buff, const int hash);
            MinimalPerfect & operator()(const char * const            text, const int hash);

            int operator()(const void * const data, size_t size) const noexcept;
            int operator()(const Memory::ReadOnlyBuffer &  buff) const noexcept;
            int operator()(const char * const              text) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MinimalPerfect);
            class Node;
            Node *root;
        };
    }
}

#endif

