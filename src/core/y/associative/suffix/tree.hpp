//! \file


#ifndef Y_Associative_Suffix_Tree_Included
#define Y_Associative_Suffix_Tree_Included 1

#include "y/container/readable.hpp"

namespace Yttrium
{

    class SuffixTree
    {
    public:
        typedef Readable<uint8_t> Path;

        explicit SuffixTree();
        virtual ~SuffixTree() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixTree);
        class Code;
        Code *code;
    };

}

#endif

