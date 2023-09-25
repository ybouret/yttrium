
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
        //______________________________________________________________________
        static inline unsigned Log(Type v) noexcept
        {
            assert(v>0);
            unsigned r=0;
            while( (v>>=1) > 0 ) ++r;
            assert( v <= (One<<r) );
            return r;
        }


        //______________________________________________________________________
        //
        //! return shift such that updated request = 2^shift
        //______________________________________________________________________
        static inline unsigned LogFor(Type &request) noexcept
        {
            assert(request<=MaxPowerOfTwo);
            unsigned shift = 0;
            unsigned bytes = 1;
            while(bytes<request) { bytes <<= 1; ++shift; }
            request = bytes;
            return shift;
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
