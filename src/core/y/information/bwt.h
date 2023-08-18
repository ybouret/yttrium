//! \file

#ifndef YACK_INFO_BWT_INCLUDED
#define YACK_INFO_BWT_INCLUDED 1


#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! forward Burrows Wheeler Transform, returns the primary index */
    size_t yack_bwt_encode(void        *output,
                           const void  *input,
                           const size_t size,
                           size_t      *indices);





    /**! reverse Burrows Wheeler Transform  using the primary index */
    void   yack_bwt_decode(void        *output,
                           const void  *input,
                           const size_t size,
                           size_t      *indices,
                           const size_t primary_index);




#if defined(__cplusplus)
}
#endif

#endif
