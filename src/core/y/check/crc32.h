
//! \file

#ifndef Y_CRC32_Included
#define Y_CRC32_Included 1

#include "y/config/starting.h"

#if defined(__cplusplus)
extern "C" {
#endif

    uint32_t Y_CRC32_Run(uint32_t    crc,
                         const void *addr,
                         size_t      size);
    
    uint32_t Y_CRC32(const void *addr, size_t size);
    
#if defined(__cplusplus)
}
#endif

#endif
