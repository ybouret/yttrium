#include "y/hashing/fnv.hpp"

namespace Yttrium
{
	namespace Hashing
	{
		
        FNV:: FNV() noexcept : Function( __length, __window ), hash(0)
		{
		}
		
        FNV:: ~FNV() noexcept
		{
			hash = 0;
		}
		
		const char * const FNV:: CallSign = "FNV";
		
		void FNV:: set() noexcept
		{
			hash = 0;
		}
        
        static const uint32_t fnv_prime = 0x811C9DC5;

		void FNV:: run(const void *buffer, size_t buflen) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t *str = (const uint8_t *)buffer;
			
			for(;buflen>0; --buflen, ++str)
			{
				hash *= fnv_prime;
				hash ^= (*str);
			}
			
		}
		
        
        
		void FNV:: get( void *output, size_t outlen ) noexcept
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

