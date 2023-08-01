#include "y/associative/suffix/tree.hpp"
#include "y/data/list.hpp"
#include "y/memory/blanks.hpp"

namespace Yttrium
{
    namespace
    {
        class SuffixNode
        {
        public:
            inline SuffixNode(SuffixNode *  parent,
                              const uint8_t b,
                              void         *d) noexcept :
            byte(b), data(d), from(parent), next(0), prev(0), chld() {}

            inline ~SuffixNode() noexcept {}


            const uint8_t      byte;
            void *             data;
            SuffixNode *       from;
            SuffixNode *       next;
            SuffixNode *       prev;
            ListOf<SuffixNode> chld;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixNode);
        };

    }

    class SuffixTree::Code
    {
    public:
        explicit Code() : pool(0) {}
        virtual ~Code() noexcept  {}

        Blanks<SuffixNode> pool;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };

    SuffixTree:: SuffixTree() : code( new Code() )
    {
    }

    SuffixTree:: ~SuffixTree() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

}
