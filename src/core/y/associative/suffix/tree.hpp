//! \file


#ifndef Y_Associative_Suffix_Tree_Included
#define Y_Associative_Suffix_Tree_Included 1

#include "y/string.hpp"

namespace Yttrium
{

    class SuffixTree
    {
    public:
        explicit SuffixTree();
        virtual ~SuffixTree() noexcept;

        void  *insert(const void *path, const size_t size, void *);
        void  *insert(const Memory::ReadOnlyBuffer &, void *);
        String pathOf(const void *nodeAddr) const;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixTree);
        class Code;
        Code *code;
    };

}

#endif

