/**!  \file */

#ifndef Y_Config_Starting_Included
#define Y_Config_Starting_Included 1

#include <assert.h>
#include "y/config/platform.h"


#include <stdlib.h>
#include <stddef.h>
#if defined(Y_STDINT)
# undef Y_STDINT
#endif

#if defined(__GNUC__)
#define Y_STDINT 1
#include <stdint.h>
#    define    Y_U64(X) X##ULL
#    define    Y_I64(X) X##LL
#endif



#if defined(_MSC_VER)
#if _MSC_VER >= 1900
#include <stdint.h>
#else

/* Microsoft built-in */
typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
#endif

#        define    Y_STDINT 1
#        define    Y_U64(X) X##ui64
#        define    Y_I64(X) X##i64

#endif


#if !defined(Y_STDINT)
#    error "no STDINT"
#endif




#endif
