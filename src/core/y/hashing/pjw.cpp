#include "y/hashing/pjw.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        PJW:: PJW() noexcept : Function( __length, __window ), hash(0)
        {
        }

        PJW:: ~PJW() noexcept
        {
            hash = 0;
        }

        
        const char * const PJW:: CallSign = "PJW";

        void PJW:: set() noexcept
        {
            hash = 0;
        }

        void PJW:: run( const void *buffer, size_t buflen ) noexcept
        {
            assert( !(buffer==NULL&&buflen>0) );
#	define BitsInUnsignedInt   ( (uint32_t)(sizeof(uint32_t) * 8) )
#	define ThreeQuarters       ( (uint32_t)((BitsInUnsignedInt  * 3) / 4))
#	define OneEighth           ( (uint32_t)(BitsInUnsignedInt / 8) )
#	define HighBits            ( (uint32_t)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth) )

            const uint8_t *str   = (const uint8_t *)buffer;
            for(;buflen>0;--buflen,++str)
            {

                hash = ( hash << OneEighth) + (*str);
                {
                    const uint32_t test = hash & HighBits;
                    if( test != 0 )
                    {
                        hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
                    }
                }

            }

        }

        void PJW:: get( void *output, size_t outlen ) noexcept
        {
            fill(output, outlen, &hash, sizeof(hash) );
        }
    }

}


#include "y/hashing/testing.hpp"

namespace Yttrium
{

    namespace Hashing
    {
        static const Testing PJW_vec[] =
        {
            { "jdfgsdhfsdfsd 6445dsfsd7fg/*/+bfjsdgf%$^", "8efdce0e" }
        };

        void PJW:: Tests()
        {
            Y_HASHING_TEST(PJW,PJW_vec);
        }


	}
	
}

