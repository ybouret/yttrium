

//! \file

#ifndef Y_Calculus_Align_Included
#define Y_Calculus_Align_Included 1

#include "y/calculus/ilog2.hpp"


#define Y_SIZE_T(X) ( static_cast<size_t>(X) ) //!< in-place size_t
#define Y_SIZE_1    Y_SIZE_T(1)                //!< in-place 1 as size_t

//! get align byte=2^LN2
#define Y_ALIGN_BYTE( LN2 ) Y_SIZE_T(Y_SIZE_1<<(LN2))

//! get align mask=(2^LN2-1)
#define Y_ALIGN_MASK( LN2 ) Y_SIZE_T( Y_ALIGN_BYTE(LN2) - Y_SIZE_1 )

//! get rounding mask 2-complement
#define Y_ALIGN_NOT2(LN2)   Y_SIZE_T( ~Y_ALIGN_MASK(LN2) )

//! get the rounded value
#define Y_ALIGN_LN2(LN2,N) ( ( Y_SIZE_T(N) + Y_ALIGN_MASK(LN2) ) & Y_ALIGN_NOT2(LN2) )

#define Y_ALIGN(N)    Y_ALIGN_LN2(0,N) //!< align on 1 byte   | 8 bits
#define Y_ALIGN2(N)   Y_ALIGN_LN2(1,N) //!< align on 2 bytes  | 16 bits
#define Y_ALIGN4(N)   Y_ALIGN_LN2(2,N) //!< align on 4 bytes  | 32 bits
#define Y_ALIGN8(N)   Y_ALIGN_LN2(3,N) //!< align on 8 bytes  | 64 bits
#define Y_ALIGN16(N)  Y_ALIGN_LN2(4,N) //!< align on 16 bytes | 128 bits
#define Y_ALIGN32(N)  Y_ALIGN_LN2(5,N) //!< align on 32 bytes | 256 bits

#define Y_MEMALIGN(N) Y_ALIGN16(N)    //!< default memory alignment

//! align on power of two SIZE
#define Y_ALIGN_ON(SIZE,N) Y_ALIGN_LN2(Yttrium::iLog2<SIZE>::Value,N)

//! align on sizeof(TYPE) = power of two
#define Y_ALIGN_TO(TYPE,N) Y_ALIGN_ON(sizeof(TYPE),N)


//! number of word to hold BYTES
#define Y_WORDS_GEQ(BYTES) (Y_ALIGN_TO(void*,(BYTES))/sizeof(void*))

//! number of words to hold TYPE
#define Y_WORDS_FOR(TYPE) Y_WORDS_GEQ(sizeof(TYPE))
#endif
