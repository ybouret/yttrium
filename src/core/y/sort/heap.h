
//! \file

#ifndef YACK_SORT_HEAP_C_INCLUDED
#define YACK_SORT_HEAP_C_INCLUDED 1


#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! item[1..n], width = sizeof(item[0]), rra[width] */
    void Y_HeapSort(void         *item,
                    const size_t  n,
                    const size_t  width,
                    void         *rra,
                    int (*proc)(const void *lhs, const void *rhs, void *args),
                    void *args);


#if defined(__cplusplus)
}
#endif


#endif
