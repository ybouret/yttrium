
//! \file

#ifndef Y_Counting_Perm_Include
#define Y_Counting_Perm_Include 1

#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! context for permutatons */
    struct Y_Perm
    {
        size_t   n; //!< among n, count will be n!
    };

    /**! set n */
    void    Y_Perm_Init(struct Y_Perm *          param, size_t n);
    
    /**! perm[1..n] = [1..n] */
    void    Yack_Perm_Boot(const struct Y_Perm * param, size_t *perm);

    /**! next permutation */
    int    Yack_Perm_Next(const struct Y_Perm * param, size_t *perm);

#if defined(__cplusplus)
}
#endif


#endif
