
/*! \file */

#ifndef Y_Memory_OutOfReach_Included
#define Y_Memory_OutOfReach_Included 1

#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void *     Y_OutOfReach_Addr(void *);
    void *     Y_OutOfReach_Zero(void *, const size_t );
    void *     Y_OutOfReach_Move(void *, const void *, const size_t);
    void *     Y_OutOfReach_Mov0(void *, void *, const size_t);
    void *     Y_OutOfReach_Swap(void *, void *, const size_t);
    ptrdiff_t  Y_OutOfReach_Diff(const void *a, const void *b); /*!< b-a */
    int        Y_OutOfReach_Is0(const void *, const size_t);

#define Y_OutOfReach_Init(FIELD) Y_OutOfReach_Zero(&FIELD,sizeof(FIELD))

#if defined(__cplusplus)
}
#endif

#endif

