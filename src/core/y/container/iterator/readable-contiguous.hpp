//! \file

#ifndef Y_Readable_Contiguous_Memory_Included
#define Y_Readable_Contiguous_Memory_Included 1

#include "y/container/iterator/linear.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! base class to form linear const iterators
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class ReadableContiguous
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T,Type); //!< aliases
        typedef Iterating::Linear<ConstType,Iterating::Forward> ConstIterator;        //!< alias
        typedef Iterating::Linear<ConstType,Iterating::Reverse> ConstReverseIterator; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        inline explicit ReadableContiguous() noexcept {} //!< setup

    public:
        inline virtual ~ReadableContiguous() noexcept {} //!< cleanup


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline ConstIterator begin() const noexcept { return ConstIterator( getBaseForward() ); } //!< begin forward, const
        inline ConstIterator end()   const noexcept { return ConstIterator( getLastForward() ); } //!< end forward, const

        inline ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator( getBaseReverse() ); } //!< begin reverse, const
        inline ConstReverseIterator rend()   const noexcept { return ConstReverseIterator( getLastReverse() ); } //!< end reverse, const




    protected:
        virtual ConstType *getBaseForward() const noexcept = 0; //!< base forward item
        virtual ConstType *getLastForward() const noexcept = 0; //!< last forward item (invalid)

        virtual ConstType *getBaseReverse() const noexcept = 0; //!< base reverse item
        virtual ConstType *getLastReverse() const noexcept = 0; //!< last reverse item (invalid)


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ReadableContiguous);


    };

   

}

#endif
