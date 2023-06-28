//! \file

#ifndef Y_Config_NoExcept_Included
#define Y_Config_NoExcept_Included 1


#include "y/config/compiler.h"

//! (A.B.C) < (D.E.F)
#define Y_VERSION_LESSER(A,B,C,D,E,F)   \
/**/ (                                     \
/**/   (A<D) ||                            \
/**/    ( (A==D) &&                        \
/**/      ( (B<E) || ( (B==E) && (C<F) ) ) \
/**/    )                                  \
/**/ )


// adapting to old g++
#if defined(Y_GNU)

#if Y_VERSION_LESSER(Y_MAJOR,Y_MINOR,Y_PATCH,4,8,1)
#define noexcept throw()
#endif

#endif

// adapting to old icc
#if defined(Y_INTEL)

#if (__ICC<=1310)
#define noexcept throw()
#endif

#endif


#endif


