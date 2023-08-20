//! \file

#ifndef Y_Information_BWT_Included
#define Y_Information_BWT_Included 1


#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    /**! forward Burrows Wheeler Transform, returns the primary index */
    size_t Y_BWT_Encode(void        *output,
                        const void  *input,
                        const size_t size,
                        size_t      *indices);





    /**! reverse Burrows Wheeler Transform  using the primary index */
    void   Y_BWT_Decode(void        *output,
                        const void  *input,
                        const size_t size,
                        size_t      *indices,
                        const size_t primary_index);




#if defined(__cplusplus)
}
#endif

#endif
