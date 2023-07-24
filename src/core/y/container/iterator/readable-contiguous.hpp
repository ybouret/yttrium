//! \file

#ifndef Y_Readable_Contiguous_Memory_Included
#define Y_Readable_Contiguous_Memory_Included 1

#include "y/container/iterator/linear.hpp"

namespace Yttrium
{
    template <typename T>
    class ReadableContiguous
    {
    public:
        Y_ARGS_EXPOSE(T,Type); //!< aliases
        typedef Iterating::Linear<ConstType,Iterating::Forward> ConstIterator;
        typedef Iterating::Linear<ConstType,Iterating::Reverse> ConstReverseIterator;

    protected:
        inline explicit ReadableContiguous() noexcept {}

    public:
        inline virtual ~ReadableContiguous() noexcept {}

        inline ConstIterator begin() const noexcept { return ConstIterator( getBaseForward() ); }
        inline ConstIterator end()   const noexcept { return ConstIterator( getLastForward() ); }

        inline ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator( getBaseReverse() ); }
        inline ConstReverseIterator rend()   const noexcept { return ConstReverseIterator( getLastReverse() ); }




    protected:
        virtual ConstType *getBaseForward() const noexcept = 0;
        virtual ConstType *getLastForward() const noexcept = 0;

        virtual ConstType *getBaseReverse() const noexcept = 0;
        virtual ConstType *getLastReverse() const noexcept = 0;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(ReadableContiguous);


    };

   

}

#endif
