#include "y/hashing/bjh32.hpp"

namespace Yttrium
{
	namespace Hashing
	{
		
		BJH32:: BJH32() noexcept : Function( __length, __window ), hash(0)
		{
		}
		
        BJH32:: ~BJH32() noexcept
		{
			hash = 0;
		}
		
        const char  BJH32:: clid[] =  "bjh32";
		
		void BJH32:: set() noexcept
		{
			hash = 0;
		}
		
		void BJH32:: run(const void *buffer, size_t buflen) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t   *key   = (const uint8_t *)buffer;
			
            while(buflen-- >0 )
            {
                hash += *(key++);
                hash += ( (hash) << 10);
                hash ^= ( (hash) >> 6 );
            }
            
		}
		
		void BJH32:: get( void *output, size_t outlen ) noexcept
		{
			hash  += ( (hash) << 3);
			hash  ^= ( (hash) >> 11);
			hash  += ( (hash) << 15);
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

