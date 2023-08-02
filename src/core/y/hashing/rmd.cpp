#include "y/hashing/rmd.hpp"
#include <cstring>

namespace Yttrium
{
    
    namespace Hashing
    {
        RMD:: ~RMD() noexcept
        {
            reset();
            memset(X,0,sizeof(X));
            memset(B,0,sizeof(B));
        }
        
#if defined(_MSC_VER)
        // init B
#pragma warning ( disable : 4351 )
#endif
        RMD:: RMD() noexcept :
        length(0),
        nx(0),
        nb(0),
        X(),
        B()
        {
            memset(X,0,sizeof(X));
            memset(B,0,sizeof(B));
        }
        
        
        void RMD:: reset() noexcept
        {
            Coerce(length) = nx = nb = 0;

        }
        
#define B2X() ( ( uint32_t(B[3]) << 24 ) |  ( uint32_t(B[2]) << 16 ) |  ( uint32_t(B[1]) << 8 ) | uint32_t(B[0]) )
        bool RMD:: store( uint8_t b ) noexcept
        {
            assert(nb<4);
            assert(nx<16);
            ++Coerce(length);
            B[nb++] = b;
            if(nb>=4)
            {
                X[nx++] = B2X();
                nb = 0;
                if( nx >= 16 )
                {
                    nx = 0;
                    return true;
                }
            }
            return false;
        }
        
        const uint32_t * RMD:: block() const noexcept
        {
            assert(0==nx);
            assert(0==nb);
            assert(length>0);
            return X;
        }
        
        const uint8_t * RMD:: flush() noexcept
        {
            assert(nx<16);
            assert(nb<4);
            for(size_t i=nb;i<4;++i)
                B[i] = 0;
            X[nx] = B2X();
            return (uint8_t *)X;
        }
        
        uint32_t RMD:: lswlen() const noexcept
        {
            return uint32_t(length);
        }
        
        uint32_t RMD:: mswlen() const noexcept
        {
            uint64_t tmp = length;
            tmp >>= 32;
            tmp &=  0xFFFFFFFFUL;
            return uint32_t(tmp);
        }
        
    }
}
