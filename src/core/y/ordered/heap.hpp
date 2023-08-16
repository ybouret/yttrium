
//! \file

#ifndef Y_Ordered_Heap_Included
#define Y_Ordered_Heap_Included 1

#include "y/ordered/priority-queue.hpp"
#include "y/type/capacity.hpp"
#include "y/comparison.hpp"

namespace Yttrium
{

    namespace Core
    {
        class Heap
        {
        public:
            static const char * const CallSign;
            virtual ~Heap() noexcept;
            explicit Heap() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Heap);
        };
    }

    template <
    typename T,
    typename RAW_BUFFER,
    typename COMPARATOR = IncreasingComparator >
    class Heap : public RAW_BUFFER
    {
    public:
        Y_ARGS_DECL(T,Type);
        typedef RAW_BUFFER RawBufferType;
        inline explicit Heap() noexcept : RawBufferType(), compare() {}
        inline explicit Heap(const size_t n, const AsCapacity_ &) noexcept : RawBufferType(n), compare() {}

        inline virtual ~Heap() noexcept {}

        inline void      insert(ParamType args)
        { this->insertWith(compare,args);   }

        inline void      insert(ParamType args, size_t n)
        { while(n-- > 0) this->insertWith(compare,args); }

        inline void      remove() noexcept
        { this->removeWith(compare); }

        inline ConstType pull()
        { return this->uprootWith(compare); }


        const COMPARATOR compare;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Heap);
    };




}


#endif
