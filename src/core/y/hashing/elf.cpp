#include "y/hashing/elf.hpp"

namespace Yttrium
{
	namespace Hashing
	{
		
        ELF:: ELF() noexcept : Function( __length, __window ), hash(0)
		{
		}
		
        ELF:: ~ELF() noexcept
		{
			hash = 0;
		}
		
        
        const char * const ELF:: CallSign = "ELF";

		void ELF:: set() noexcept
		{
			hash = 0;
		}
		
		void ELF:: run( const void *buffer, size_t buflen ) noexcept
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t *str   = (const uint8_t *)buffer;
			for(;buflen>0;--buflen,++str)
			{
				uint32_t x = 0;
				hash = (hash << 4) + (*str);
				if((x = hash & 0xF0000000L) != 0)
				{
					hash ^= (x >> 24);
				}
				hash &= ~x;
			}		
		}
		
		void ELF:: get( void *output, size_t outlen ) noexcept
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

