
//! \file

#ifndef Y_Calculus_Base2_Included
#define Y_Calculus_Base2_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! base2 operations
    //
    //__________________________________________________________________________
    template <typename Type>
    struct Base2
    {
        static const bool     Signed           = IsSigned<Type>::Value;             //!< alias
        static const unsigned MaxShift         = (sizeof(Type)<<3) - (Signed?2:1); //!< alias
        static const unsigned MaxSlots         = MaxShift+1;                        //!< alias
        static const Type     One              = 1;                                 //!< alias
        static const Type     MaxPowerOfTwo    = (One << MaxShift);                //!< alias


        //______________________________________________________________________
        //
        //! compute r such that v <= (1<<r)
        //__________________________________________________________________________
        static inline unsigned Log(Type v) noexcept
        {
            assert(v>0);
            unsigned r=0;
            while( (v>>=1) > 0 ) ++r;
            assert( v <= (One<<r) );
            return r;
        }


    };


    //! power of two detection
    template <typename T> inline
    bool IsPowerOfTwo( T v ) noexcept
    {
        return (v>0) && !( v & (v - Base2<T>::One) );
    }

    //! next power of two of v<=max_power_of_two
    template <typename T>
    inline T NextPowerOfTwo( T v ) noexcept
    {
        if(v>0)
        {
            assert(v<=Base2<T>::MaxPowerOfTwo);
            --v;
            for( size_t shift = 1; shift <= (sizeof(T)<<2); shift <<= 1 )
            {
                v |= ( v >> shift );
            }
            return ++v;
        }
        else
        {
            return Base2<T>::One;
        }
    }

    //! most_significant_bit of v>=0
    template <typename T>
    inline T MostSignificantBit(const T v) noexcept
    {
        T mask = Base2<T>::MaxPowerOfTwo;
        while(mask)
        {
            if(0!=(v&mask)) return mask;
            mask >>= 1;
        }
        return 0;
    }

    //! bits necessary to write v
    template <typename T>
    inline unsigned BitsFor(const T v) noexcept
    {
        T        mask = Base2<T>::MaxPowerOfTwo;
        unsigned nbit = 8*sizeof(T);
        while(mask)
        {
            if(0!=(v&mask)) return nbit;
            mask >>= 1;
            --nbit;
        }
        return 0;
    }

    //! previous power of two, v>0
    template <typename T>
    inline T PrevPowerOfTwo( const T v ) noexcept
    {
        assert(v>0);
        T mask = Base2<T>::MaxPowerOfTwo;;
        while(true)
        {
            if(0!=(v&mask)) return mask;
            mask >>= 1;
        }
    }

    


}
#endif
