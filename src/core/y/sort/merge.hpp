
//! \file

#ifndef Y_Sort_Merge_Included
#define Y_Sort_Merge_Included 1

#include "y/data/list.hpp"

namespace Yttrium
{

    struct MergeSort
    {
        template <typename LIST>
        class Pair
        {
        public:
            LIST  lhs;
            LIST  rhs;
            void *src;

            inline  Pair() noexcept : lhs(), rhs(), src(0) {}
            inline ~Pair() noexcept {}

            template <typename SOURCE>
            void collect(SOURCE &F) noexcept
            {
                assert(0==lhs.size);
                assert(0==rhs.size);
                assert(0==src);
                assert(F.size>=2);
                std::cerr << "#" << F.size<< " => ";
                ListOps::Divide(lhs,rhs,F);
                src = &F;
                assert(0==F.size);
                std::cerr << lhs.size << " | " << rhs.size << std::endl;
            }

            template <typename SOURCE, typename COMPARE_NODES>
            void reduce(COMPARE_NODES &compareNodes)
            {
                assert(0!=src);
                assert(lhs.size>0);
                assert(rhs.size>0);
                SOURCE &F = *static_cast<SOURCE *>(src);
                ListOps::Fusion(F,lhs,rhs,compareNodes);
                std::cerr << F << std::endl;
                src = 0;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pair);
        };


        template <typename LIST, typename COMPARE_NODES> static inline
        void Call(LIST &L, COMPARE_NODES &compareNodes)
        {
            // internal definitions
            typedef typename LIST::NodeType NodeType;
            typedef ListOf<NodeType>        ListType;
            typedef Pair<ListType>          PairType;

            if(L.size<2) return;

            PairType  stk[32];
            {
                PairType *top = stk;
                {
                    PairType *bot = top;
                    top->collect(L);
                   // while(true)
                    do
                    {
                        std::cerr << "processing bot: " << bot->lhs.size << " , " << bot->rhs.size << std::endl;
                        if(bot->lhs.size>1) { std::cerr << "div lhs: "; (++top)->collect(bot->lhs); }
                        if(bot->rhs.size>1) { std::cerr << "div rhs: "; (++top)->collect(bot->rhs); }
                        //++bot;
                        //if(bot>top) break;
                    }
                    while(++bot<=top);
                }
                std::cerr << "all divided in " << top-stk << " lists" << std::endl;
                while(top>stk)
                {
                    top->template reduce<ListType,COMPARE_NODES>(compareNodes);
                    --top;
                }
                assert(stk==top);
            }
            stk->template reduce<LIST,COMPARE_NODES>(compareNodes);
        }


    };

}

#endif
