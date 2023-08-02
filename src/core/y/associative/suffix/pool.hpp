//! \file


#ifndef Y_Associative_Suffix_Pool_Included
#define Y_Associative_Suffix_Pool_Included 1

#include "y/memory/blanks.hpp"

namespace Yttrium
{
    template <typename NODE>
    class SuffixPool : public Blanks<NODE>
    {
    public:

        using Blanks<NODE>::zacquire;
        using Blanks<NODE>::zrelease;

        inline explicit SuffixPool() noexcept : Blanks<NODE>(0) {}
        inline virtual ~SuffixPool() noexcept {}

        template <typename KEY, typename T> inline
        NODE * create(typename TypeTraits<KEY>::ParamType k,
                      typename TypeTraits<T>::  ParamType t)
        {
            void *node = zacquire();
            try  { return new (node) NODE(k,t); }
            catch(...) { zrelease(node); throw; }
        }

        template <typename T> inline
        NODE * create(typename TypeTraits<T>:: ParamType t)
        {
            void *node = zacquire();
            try    { return new (node) NODE(t); }
            catch(...) { zrelease(node); throw; }
        }

        inline NODE *duplicate(const NODE *user)
        {
            assert(0!=user); void *node = zacquire();
            try { return new (node) NODE(*user); }
            catch(...) { zrelease(node); throw; }
        }

        inline void destruct( NODE *node ) noexcept
        { assert(0!=node); zrelease( Destructed(node) ); }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixPool);
    };

}

#endif

