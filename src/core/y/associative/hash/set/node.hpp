//! \file


#ifndef Y_Associative_Hash_Set_Node_Included
#define Y_Associative_Hash_Set_Node_Included 1

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
    class HashSetNode
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
        inline   HashSetNode(ParamType t) :
        next(0), prev(0), data(t), key(data.key())
        {
        }

        //! copy using data copy constructor
        inline HashSetNode(const HashSetNode &node) :
        next(0), prev(0), data(node.data), key(data.key())
        {
        }

        //! cleanup
        inline ~HashSetNode() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline Type      & operator*()       noexcept { return data; } //!< access
        inline ConstType & operator*() const noexcept { return data; } //!< access

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        HashSetNode *next; //!< for list
        HashSetNode *prev; //!< for list
        MutableType  data; //!< data
        ConstKey &   key;  //!< key from data

    private:
        Y_DISABLE_ASSIGN(HashSetNode);
    };

}

#endif

