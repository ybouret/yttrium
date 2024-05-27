#include "y/apk/component.hpp"


namespace Yttrium
{
    namespace APK
    {

        void * Component:: Tuned(const InnerState st, uint64_t &qw, size_t &nc) noexcept
        {
            void * const addr = &qw;
            nc                = 0;
            if(qw>0)
            {
                switch(st)
                {
                    case AsBytes: {
                        uint8_t * const p    = (uint8_t *) addr;
                        const uint8_t   w[8] = {
                            uint8_t(qw),     uint8_t(qw>>8),  uint8_t(qw>>16), uint8_t(qw>>24),
                            uint8_t(qw>>32), uint8_t(qw>>40), uint8_t(qw>>48), uint8_t(qw>>56)
                        };
                        if( ( p[0] = w[0] ) > 0) nc=1;
                        if( ( p[1] = w[1] ) > 0) nc=2;
                        if( ( p[2] = w[2] ) > 0) nc=3;
                        if( ( p[3] = w[3] ) > 0) nc=4;
                        if( ( p[4] = w[4] ) > 0) nc=5;
                        if( ( p[5] = w[5] ) > 0) nc=6;
                        if( ( p[6] = w[6] ) > 0) nc=7;
                        if( ( p[7] = w[7] ) > 0) nc=8;
                        assert(nc>=1);
                    } break;

                    case AsNum16: {
                        uint16_t * const p    = (uint16_t *) addr;
                        const uint16_t   w[4] = { uint16_t(qw), uint16_t(qw>>16), uint16_t(qw>>32), uint16_t(qw>>48) };
                        if( ( p[0] = w[0] ) > 0) nc=1;
                        if( ( p[1] = w[1] ) > 0) nc=2;
                        if( ( p[2] = w[2] ) > 0) nc=3;
                        if( ( p[3] = w[3] ) > 0) nc=4;
                        assert(nc>=1);
                    } break;

                    case AsNum32: {
                        uint32_t * const p    = (uint32_t *) addr;
                        const uint32_t   w[2] = { uint32_t(qw), uint32_t(qw>>32) };
                        if( ( p[0] = w[0] ) > 0) nc=1;
                        if( ( p[1] = w[1] ) > 0) nc=2;
                        assert(nc>=1);
                    } break;

                    case AsNum64:
                        nc=1;
                        break;
                }
                assert(nc>=1);
            }

            return addr;
        }


    }

}

