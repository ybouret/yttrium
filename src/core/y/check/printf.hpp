
//! \file

#ifndef Y_Check_Printf_Included
#define Y_Check_Printf_Included 1

//! \warning for C++ methods, increase count by 1!
#if defined(__GNUC__)
#    define Y_PRINTF_CHECK(i,j) __attribute__ ((format (printf, i, j)))
#else
#    define Y_PRINTF_CHECK(i,j)
#endif

//! default C++ formating methods
#define Y_PRINTF_API Y_PRINTF_CHECK(2,3)


#endif

