
#ifndef Y_Writable_Contiguous_Memory_Included
#define Y_Writable_Contiguous_Memory_Included 1

#include "y/container/iterator/readable-contiguous.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! base class to form linear iterators
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class WritableContiguous : public ReadableContiguous<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T,Type); //!< aliases
        typedef Iterating::Linear<Type,Iterating::Forward>      Iterator;             //!< alias
        typedef Iterating::Linear<Type,Iterating::Reverse>      ReverseIterator;      //!< alias
        typedef Iterating::Linear<ConstType,Iterating::Forward> ConstIterator;        //!< alias
        typedef Iterating::Linear<ConstType,Iterating::Reverse> ConstReverseIterator; //!< alias

    protected:
        inline explicit WritableContiguous() noexcept : ReadableContiguous<T>() {} //!< setup
        using ReadableContiguous<T>::getBaseForward;
        using ReadableContiguous<T>::getLastForward;
        using ReadableContiguous<T>::getBaseReverse;
        using ReadableContiguous<T>::getLastReverse;

    public:
        inline virtual ~WritableContiguous() noexcept {} //!< cleanup

        inline Iterator      begin()       noexcept { return Iterator( (MutableType *)(getBaseForward()) ); } //!< begin forward
        inline Iterator      end()         noexcept { return Iterator( (MutableType *)(getLastForward()) ); } //!< end forward

        inline ConstIterator begin() const noexcept { return ConstIterator( getBaseForward() ); } //!< begin forward, const
        inline ConstIterator end()   const noexcept { return ConstIterator( getLastForward() ); } //!< end forward, const

        inline ReverseIterator rbegin()             noexcept { return ReverseIterator( (MutableType *)(getBaseReverse()) ); } //!< begin reverse
        inline ReverseIterator rend()               noexcept { return ReverseIterator( (MutableType *)(getLastReverse()) ); } //!< end reverse

        inline ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator( getBaseReverse() ); } //!< begin reverse, const
        inline ConstReverseIterator rend()   const noexcept { return ConstReverseIterator( getLastReverse() ); } //!< end reverse, const


    private:
        Y_DISABLE_COPY_AND_ASSIGN(WritableContiguous);


    };

}

#endif

