//! \file

#ifndef Y_Contiguous_Memory_Included
#define Y_Contiguous_Memory_Included 1

#include "y/container/iterator/linear.hpp"

namespace Yttrium
{
    template <typename T>
    class ContiguousReadable
    {
    public:
        Y_ARGS_EXPOSE(T,Type); //!< aliases
        typedef Iterating::Linear<ConstType,Iterating::Forward> ConstIterator;
        typedef Iterating::Linear<ConstType,Iterating::Reverse> ConstReverseIterator;

    protected:
        inline explicit ContiguousReadable() noexcept {}

    public:
        inline virtual ~ContiguousReadable() noexcept {}

        inline ConstIterator begin() const noexcept { return ConstIterator( getBaseForward() ); }
        inline ConstIterator end()   const noexcept { return ConstIterator( getLastForward() ); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ContiguousReadable);
        virtual ConstType *getBaseForward() const noexcept = 0;
        virtual ConstType *getLastForward() const noexcept = 0;

    };

    template <typename T>
    class ContiguousWritable
    {
    public:
        Y_ARGS_EXPOSE(T,Type); //!< aliases

        typedef Iterating::Linear<Type,Iterating::Forward>      Iterator;
        typedef Iterating::Linear<Type,Iterating::Reverse>      ReverseIterator;
        typedef Iterating::Linear<ConstType,Iterating::Forward> ConstIterator;
        typedef Iterating::Linear<ConstType,Iterating::Reverse> ConstReverseIterator;

    protected:
        inline explicit ContiguousWritable() noexcept {}

    public:
        inline virtual ~ContiguousWritable() noexcept {}

        inline Iterator      begin()       noexcept { return Iterator( (MutableType *)(getBaseForward()) ); }
        inline Iterator      end()         noexcept { return Iterator( (MutableType *)(getLastForward()) ); }
        inline ConstIterator begin() const noexcept { return ConstIterator( getBaseForward() ); }
        inline ConstIterator end()   const noexcept { return ConstIterator( getLastForward() ); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ContiguousWritable);
        virtual ConstType *getBaseForward() const noexcept = 0;
        virtual ConstType *getLastForward() const noexcept = 0;

    };


}

#endif
