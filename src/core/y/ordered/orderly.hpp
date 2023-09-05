
//! \file

#ifndef Y_Ordered_Locate_Included
#define Y_Ordered_Locate_Included 1

#include "y/type/signs.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! locate/insert withing ordered arrays
    //
    //
    //__________________________________________________________________________
    struct Orderly
    {

        //______________________________________________________________________
        //
        //! locate data withing an ordered array
        /**
         \param ipos    insertion/current position
         \param args    argument to insert/look-for
         \param data    data[0..size-1]
         \param size    current number of objects in array
         \param compare SignType compare(args,data[.])
         */
        //______________________________________________________________________
        template <
        typename T,
        typename U,
        typename COMPARE>
        static inline
        bool Locate(size_t      &ipos,
                    T           &args,
                    U           *data,
                    const size_t size,
                    COMPARE     &compare)
        {

            //------------------------------------------------------------------
            //
            // special cases
            //
            //------------------------------------------------------------------
            switch(size)
            {
                case 0: ipos=0; return false;
                case 1:
                    switch(compare(args,data[0]))
                    {
                        case __Zero__: ipos=0; return true;  // args == data[0]
                        case Positive: ipos=1; return false; // args > data[0]
                        case Negative: ipos=0; break;        // args < data[0]

                    }
                    return false;
                default:
                    break;
            }

            // initialize bisection
            assert(size>=2);
            switch(compare(args,data[0]))
            {
                case Negative: ipos=0; return false; // args <  data[0]
                case __Zero__: ipos=0; return true;  // args == data[0]
                case Positive: break;                // args > data[0]
            }

            size_t upper = size-1;
            switch(compare(args,data[upper]))
            {
                case Negative: break;                      // args <  data[upper]
                case __Zero__: ipos = upper; return true;  // args == data[upper]
                case Positive: ipos = size;  return false; // args > data[upper]
            }

            // bisection
            size_t lower = 0;
            while(upper-lower>1)
            {
                assert(compare(args,data[lower]) == Positive);
                assert(compare(args,data[upper]) == Negative);
                const size_t probe=(lower+upper)>>1;
                switch( compare(args,data[probe]) )
                {
                    case __Zero__: ipos  = probe; return true;
                    case Negative: upper = probe; continue;
                    case Positive: lower = probe; continue;
                }
            }
            assert(compare(args,data[lower]) == Positive);
            assert(compare(args,data[upper]) == Negative);
            ipos = upper;
            return false;
        }

        //______________________________________________________________________
        //
        //
        //
        //! insert data withing an ordered array
        //
        //
        //______________________________________________________________________
        template <typename T, typename U> static inline
        void Insert(const size_t ipos,
                    T           &args,
                    U           *data,
                    size_t      &size)
        {
            assert(0!=data);
            if(ipos>=size)
            {
                // 'pushTail'
                new (data+size) U(args);
            }
            else
            {
                const size_t bytes = (size-ipos) * sizeof(U);
                U           *source = data+ipos;
                U           *target = source+1;
                Memory::OutOfReach::Move(target,source,bytes);
                try { new (source) U(args); }
                catch(...)
                {
                    Memory::OutOfReach::Move(source,target,bytes);
                    throw;
                }

            }
            ++size;
        }


        //______________________________________________________________________
        //
        //
        //! remove data at given ipos
        //
        //______________________________________________________________________
        template <typename U> static inline
        void Remove(const size_t ipos,
                    U           *data,
                    size_t      &size) noexcept
        {
            assert(ipos<size);
            assert(0!=data);
            const size_t bytes  = (--size - ipos) * sizeof(U);
            U           *target = data + ipos;
            Memory::OutOfReach::Move( Destructed(target), target+1, bytes);

        }

        //______________________________________________________________________
        //
        //
        //! helper to cleanup data
        //
        //______________________________________________________________________
        template <typename U> static inline
        void Finish(U *data, size_t &size) noexcept
        { while(size>0) Memory::OutOfReach::Naught(&data[--size]); }

        

    };

}

#endif

