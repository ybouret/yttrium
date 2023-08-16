
//! \file

#ifndef Y_Ordered_Heap_Included
#define Y_Ordered_Heap_Included 1

#include "y/ordered/priority-queue.hpp"
#include "y/type/capacity.hpp"
#include "y/comparison.hpp"

namespace Yttrium
{

    
    //__________________________________________________________________________
    //
    //
    //
    //! Parametrized Heap
    //
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename RAW_BUFFER,
    typename COMPARATOR = IncreasingComparator >
    class Heap : public RAW_BUFFER
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_DECL(T,Type);              //!< aliases
        typedef RAW_BUFFER RawBufferType; //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup empty
        inline explicit Heap() noexcept : RawBufferType(), compare() {}

        //! setup with capacity
        inline explicit Heap(const size_t n, const AsCapacity_ &) noexcept : RawBufferType(n), compare() {}

        //! cleanup
        inline virtual ~Heap() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! insert object
        inline void      insert(ParamType args)
        { this->insertWith(compare,args);   }

        //! insert mutliple objects
        inline void      insert(ParamType args, size_t n)
        { while(n-- > 0) this->insertWith(compare,args); }


        //! remove top object
        inline void      remove() noexcept
        { this->removeWith(compare); }

        //! pull top object
        inline ConstType pull()
        { return this->uprootWith(compare); }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const COMPARATOR compare; //!< comparator for priority queue
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Heap);
    };




}


#endif
