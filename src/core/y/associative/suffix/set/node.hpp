
//! \file


#ifndef Y_Associative_Suffix_Set_Node_Included
#define Y_Associative_Suffix_Set_Node_Included 1

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
    class SuffixSetNode
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

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup from object copy
        inline   SuffixSetNode(ParamType t) :
        next(0), prev(0), knot(0),
        data(t), key(data.key())
        {
        }

        //! copy using data copy constructor
        inline SuffixSetNode(const SuffixSetNode &node) :
        next(0), prev(0), knot(0),
        data(node.data), key(data.key())
        {
        }

        //! cleanup
        inline ~SuffixSetNode() noexcept {}


        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        SuffixSetNode *next; //!< for list
        SuffixSetNode *prev; //!< for list
        void          *knot; //!< for tree
        MutableType    data; //!< data
        ConstKey      &key;  //!< key from data

    private:
        Y_DISABLE_ASSIGN(SuffixSetNode);
    };

}

#endif

