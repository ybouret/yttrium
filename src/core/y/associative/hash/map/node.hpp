//! \file


#ifndef Y_Associative_Hash_Map_Node_Included
#define Y_Associative_Hash_Map_Node_Included 1

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
    class HashMapNode
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
        inline   HashMapNode(ParamKey k, ParamType t) :
        next(0), prev(0), data(t), key(k)
        {
        }

        //! copy using key/data copy constructor
        inline HashMapNode(const HashMapNode &node) :
        next(0), prev(0), data(node.data), key(node.key)
        {
        }


        //! cleanup
        inline ~HashMapNode() noexcept {}

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
        HashMapNode *next; //!< for list
        HashMapNode *prev; //!< for list
        MutableType  data; //!< data
        ConstKey     key;  //!< key

    private:
        Y_DISABLE_ASSIGN(HashMapNode);
    };

}

#endif
