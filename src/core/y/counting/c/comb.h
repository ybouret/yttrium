//! \file

#ifndef YACK_COUNTING_COMB_INCLUDED
#define YACK_COUNTING_COMB_INCLUDED 1

#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! context for combinations */
    struct Y_Comb
    {
        size_t n;        //!< width
        size_t k;        //!< space
        size_t nmk;      //!< n-k
    };

    /**! setup */
    void Y_Comb_Init(struct Y_Comb *       param, const size_t n, const size_t k);

    /**! comb[1..k]=[1..k]*/
    void Y_Comb_Boot(const struct Y_Comb * param, size_t *comb);

    /**! next combination */
    int  Y_Comb_Next(const struct Y_Comb * param, size_t *comb);

#if defined(__cplusplus)
}
#endif


#endif
