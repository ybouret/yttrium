
//! \file


#ifndef Y_Associative_Suffix_Set_Node_Included
#define Y_Associative_Suffix_Set_Node_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{
    template <typename KEY, typename T>
    class SuffixSetNode
    {
    public:
        Y_ARGS_DECL(T,Type);
        Y_ARGS_DECL(KEY,Key);

        inline   SuffixSetNode(ParamType t) :
        next(0), prev(0), knot(0),
        data(t), key(data.key())
        {
        }

        inline ~SuffixSetNode() noexcept {}

        inline SuffixSetNode(const SuffixSetNode &node) :
        next(0), prev(0), knot(0),
        data(node.data), key(data.key())
        {
        }

        SuffixSetNode *next;
        SuffixSetNode *prev;
        void          *knot;
        MutableType    data;
        ConstKey      &key;

    private:
        Y_DISABLE_ASSIGN(SuffixSetNode);
    };

}

#endif

