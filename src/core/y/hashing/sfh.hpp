//! \file
#ifndef Y_HASHING_SFH_INCLUDED
#define Y_HASHING_SFH_INCLUDED 1

#include "y/hashing/function.hpp"

namespace Yttrium
{
	namespace Hashing
	{

        //! Super Fast Hash
		class SFH : public Function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(SFH,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(SFH);
		};
		
	}
}

#endif
