
#ifndef Y_Writable_Contiguous_Memory_Included
#define Y_Writable_Contiguous_Memory_Included 1

#include "y/container/iterator/readable-contiguous.hpp"

namespace Yttrium
{

    template <typename T>
    class WritableContiguous : public ReadableContiguous<T>
    {
    public:


        Y_ARGS_EXPOSE(T,Type); //!< aliases

        typedef Iterating::Linear<Type,Iterating::Forward>      Iterator;
        typedef Iterating::Linear<Type,Iterating::Reverse>      ReverseIterator;
        typedef Iterating::Linear<ConstType,Iterating::Forward> ConstIterator;
        typedef Iterating::Linear<ConstType,Iterating::Reverse> ConstReverseIterator;

    protected:
        inline explicit WritableContiguous() noexcept {}
        using ReadableContiguous<T>::getBaseForward;
        using ReadableContiguous<T>::getLastForward;
        using ReadableContiguous<T>::getBaseReverse;
        using ReadableContiguous<T>::getLastReverse;

    public:
        inline virtual ~WritableContiguous() noexcept {}

        inline Iterator      begin()       noexcept { return Iterator( (MutableType *)(getBaseForward()) ); }
        inline Iterator      end()         noexcept { return Iterator( (MutableType *)(getLastForward()) ); }
        inline ConstIterator begin() const noexcept { return ConstIterator( getBaseForward() ); }
        inline ConstIterator end()   const noexcept { return ConstIterator( getLastForward() ); }

        inline ReverseIterator rbegin()             noexcept { return ReverseIterator( (MutableType *)(getBaseReverse()) ); }
        inline ReverseIterator rend()               noexcept { return ReverseIterator( (MutableType *)(getLastReverse()) ); }

        inline ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator( getBaseReverse() ); }
        inline ConstReverseIterator rend()   const noexcept { return ConstReverseIterator( getLastReverse() ); }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(WritableContiguous);


    };

}

#endif

