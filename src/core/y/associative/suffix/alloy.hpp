
//! \file


#ifndef Y_Associative_Suffix_Alloy_Included
#define Y_Associative_Suffix_Alloy_Included 1

#include "y/associative/suffix/tree.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Alloy from a SuffixTree
    //
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename NODE,
    class    INTERFACE,
    typename BASE>
    class SuffixAlloy :
    public INTERFACE,
    public SuffixTree<KEY,T,NODE>,
    public BASE
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

        typedef SuffixTree<KEY,T,NODE> TreeType; //!< alias
        using TreeType::list;
        using TreeType::pool;
        using TreeType::tree;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual const char * callSign()  const noexcept { return BASE::CallSign; }      //!< callSign
        inline virtual size_t       size()      const noexcept { return list.size; }           //!< list size
        inline virtual size_t       capacity()  const noexcept { return list.size+pool.available(); } //!< capacity
        inline virtual size_t       available() const noexcept { return pool.available(); }           //!< available
        inline virtual void         free()            noexcept { this->softReset(); }          //!< free content, keep memory
        inline virtual void         release()         noexcept { this->hardReset(); }          //!< release all possible memory
        inline virtual void         reserve(size_t n) noexcept { pool.reserve(n); }            //!< reserve blocks

        //! search CONST object address by its key
        inline virtual ConstType *search(ParamKey key) const noexcept
        {
            const void *nodeAddr = tree.search(key);
            if(!nodeAddr) return 0;
            return & (**static_cast<const NODE *>(nodeAddr));
        }

        //! search object address by its key
        inline virtual Type *search(ParamKey key) noexcept
        {
            void *nodeAddr = (void*) tree.search(key);
            if(!nodeAddr) return 0;
            return & (**static_cast<NODE *>(nodeAddr));
        }

        //! try to remove an object by its key
        inline virtual bool remove(ParamKey key) noexcept
        {
            const void *nodeAddr = tree.search(key);
            if(!nodeAddr) return false;
            NODE *node = (NODE *)nodeAddr; assert(0!=node->knot);
            tree.loosen(node->knot);
            pool.free( list.pop(node) );
            return true;
        }
        


        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! cleanup
        inline virtual ~SuffixAlloy() noexcept {}

    protected:

        //! setup empty
        inline explicit SuffixAlloy() noexcept :
        INTERFACE(),
        TreeType(),
        BASE()
        {
        }

        //! copy
        inline SuffixAlloy(const SuffixAlloy &other) noexcept :
        Identifiable(), Collection(),
        INTERFACE(),
        TreeType(other),
        BASE()
        {
        }




    private:
        Y_DISABLE_ASSIGN(SuffixAlloy);
    };
    

}

#endif

