#include "y/hashing/function.hpp"
#include <cstring>

namespace Yttrium
{

	namespace Hashing
	{

		Function:: ~Function() noexcept
        {
            Coerce(length) = 0;
            Coerce(window) = 0;
        }

		Function:: Function( size_t L, size_t W ) noexcept : length(L), window(W)
		{
		}


		void Function:: fill( void *buffer, size_t buflen,	const void *output, size_t outlen ) noexcept
		{
			assert( !(buffer==NULL && buflen > 0 ) );
			assert( output != NULL );
			assert( outlen > 0 );
			
			if( buflen <= outlen )
				memcpy( buffer, output, buflen );
			else
			{
				assert( buflen > outlen );
				uint8_t       *p = (uint8_t *)buffer;
				while( buflen > outlen )
				{
					memcpy(p,output,outlen);
					p      += outlen;
					buflen -= outlen;
				}
				assert(buflen<=outlen);
				memcpy(p,output,buflen);
			}
		}
        
        void Function:: operator()(const void *block_addr, const size_t block_size) noexcept
        {
            assert( !(NULL==block_addr && block_size>0 ) );
            run(block_addr,block_size);
        }

        void Function:: operator()(const char *msg) noexcept
        {
            if(msg)
            {
                run(msg,strlen(msg));
            }
        }

        
        void Function:: block(void *output, size_t outlen, const void *block_addr, const size_t block_size) noexcept
        {
            assert( !(NULL==block_addr && block_size>0 ) );
            set();
            run(block_addr,block_size);
            get(output,outlen);
        }


        
        void Function:: Flock(void *output, size_t outlen, const char *msg) noexcept
        {
            set();
            if(msg)
            {
                run(msg,strlen(msg));
            }
            get(output,outlen);
        }
        
	}

}

#include "y/memory/buffer/ro.hpp"


namespace Yttrium
{

    namespace Hashing
    {

        void Function:: operator()(const Memory::ReadOnlyBuffer &buff) noexcept
        {
            run( buff.ro_addr(), buff.measure() );
        }
        
        void Function:: block(void *output, size_t outlen, const Memory::ReadOnlyBuffer &buff) noexcept
        {
            set();
            run( buff.ro_addr(), buff.measure() );
            get(output,outlen);
        }

    }

}
