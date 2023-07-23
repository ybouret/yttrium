//! \file

#ifndef YACK_COUNTING_INTS_INCLUDED
#define YACK_COUNTING_INTS_INCLUDED 1

#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif
    
    /**! context for integer partitions */
    struct Y_Part
    {
        size_t   n; //!< the positive integer
        size_t   k; //!< current number of  parts
    };

    /**! intialize */
    void    Y_Part_Init(struct Y_Part *param, size_t n);
   
    /**! ints[1] = [n], k=1 */
    void    Y_Part_Boot(struct Y_Part *param, size_t *ints);
    
    /**! next partition */
    int     Y_Part_Next(struct Y_Part *param, size_t *ints);
    

#if defined(__cplusplus)
}
#endif


#endif
