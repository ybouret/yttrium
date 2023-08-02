//! \file
#ifndef Y_HASHING_CRC16_INCLUDED
#define Y_HASHING_CRC16_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{
        //! crc16
		class CRC16 : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(CRC16,2,2);
			
		private:
			uint16_t    hash;
			Y_DISABLE_COPY_AND_ASSIGN(CRC16);
		};
		
	}
}

#endif
