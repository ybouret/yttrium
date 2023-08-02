//! \file


#ifndef Y_Associative_Suffix_Pool_Included
#define Y_Associative_Suffix_Pool_Included 1

#include "y/memory/blanks.hpp"
#include "y/type/traits.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Pool of nodes for Suffix Trees
    //
    //
    //__________________________________________________________________________
    template <typename NODE>
    class SuffixPool : public Blanks<NODE>
    {
    public:
        using Blanks<NODE>::zacquire;
        using Blanks<NODE>::zrelease;
        using Blanks<NODE>::zdiscard;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit SuffixPool() noexcept : Blanks<NODE>(0) {}
        inline virtual ~SuffixPool() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! construct NODE for a Map
        //______________________________________________________________________
        template <typename KEY, typename T> inline
        NODE * create(typename TypeTraits<KEY>::ParamType k,
                      typename TypeTraits<T>::  ParamType t)
        {
            void *node = zacquire();
            try  { return new (node) NODE(k,t); }
            catch(...) { zrelease(node); throw; }
        }

        //______________________________________________________________________
        //
        //! construct NODE for a Set
        //______________________________________________________________________
        template <typename T> inline
        NODE * create(typename TypeTraits<T>:: ParamType t)
        {
            void *node = zacquire();
            try    { return new (node) NODE(t); }
            catch(...) { zrelease(node); throw; }
        }


        //______________________________________________________________________
        //
        //! duplicate NODE based on NODE copy constructor
        //______________________________________________________________________
        inline NODE *duplicate(const NODE *user)
        {
            assert(0!=user); void *node = zacquire();
            try { return new (node) NODE(*user); }
            catch(...) { zrelease(node); throw; }
        }

        //______________________________________________________________________
        //
        //! destruct and store block inside pool
        //______________________________________________________________________
        inline void destruct( NODE *node ) noexcept
        { assert(0!=node); zrelease( Destructed(node) ); }

        //______________________________________________________________________
        //
        //! destruct and release block to system's memory
        //______________________________________________________________________
        inline void vaporize( NODE *node ) noexcept
        { assert(0!=node); zdiscard( Destructed(node) ); }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixPool);
    };

}

#endif

