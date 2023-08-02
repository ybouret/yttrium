//! \file


#ifndef Y_Associative_Suffix_Map_Node_Included
#define Y_Associative_Suffix_Map_Node_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    template <typename KEY, typename T>
    class SuffixMapNode
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

        inline   SuffixMapNode(ParamKey k, ParamType t) :
        next(0), prev(0), knot(0),
        data(t), key(k)
        {
        }

        inline SuffixMapNode(const SuffixMapNode &node) :
        next(0), prev(0), knot(0),
        data(node.data), key(node.key)
        {
        }


        inline ~SuffixMapNode() noexcept {}

        SuffixMapNode *next;
        SuffixMapNode *prev;
        void          *knot;
        MutableType    data;
        ConstKey       key;

    private:
        Y_DISABLE_ASSIGN(SuffixMapNode);
    };

}

#endif
