//! \file
#ifndef Y_HASHING_RMD160_INCLUDED
#define Y_HASHING_RMD160_INCLUDED 1


#include "y/hashing/function.hpp"
#include "y/hashing/rmd.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        //! RipeMD 160
        class RMD160 : public Function
		{
		public:
			Y_HASHING_FUNCTION_DECL(RMD160,20,64); //!< specs
			static const size_t RMDsize=160;       //!< bits
            
            
		private:
			Y_DISABLE_COPY_AND_ASSIGN(RMD160);
            RMD      RMD_;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
