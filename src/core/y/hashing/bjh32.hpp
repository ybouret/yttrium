//! \file
#ifndef Y_HASHING_BJH32_INCLUDED
#define Y_HASHING_BJH32_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{

        //! Bob Jenkin's
		class BJH32 : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(BJH32,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(BJH32);
		};
		
	}
}

#endif
