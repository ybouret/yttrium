//! \file


#ifndef Y_Associative_Suffix_Map_Node_Included
#define Y_Associative_Suffix_Map_Node_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! internal Node for SuffixMap
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class SuffixMapNode
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);  //!< aliases
        Y_ARGS_DECL(KEY,Key); //!< aliases

        //! initialize
        inline   SuffixMapNode(ParamKey k, ParamType t) :
        next(0), prev(0), knot(0),
        data(t), key(k)
        {
        }

        //! copy using key/data copy constructor
        inline SuffixMapNode(const SuffixMapNode &node) :
        next(0), prev(0), knot(0),
        data(node.data), key(node.key)
        {
        }


        //! cleanup
        inline ~SuffixMapNode() noexcept {}

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        SuffixMapNode *next; //!< for list
        SuffixMapNode *prev; //!< for list
        void          *knot; //!< for tree
        MutableType    data; //!< data
        ConstKey       key;  //!< key

    private:
        Y_DISABLE_ASSIGN(SuffixMapNode);
    };

}

#endif
