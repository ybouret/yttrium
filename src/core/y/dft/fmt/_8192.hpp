//! \file
#ifndef Y_DFT_Fmt8192_Included
#define Y_DFT_Fmt8192_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	struct DFT_Fmt8192 {
		static const unsigned Count=4032;     //!< count
		static const uint16_t Table[4032][2]; //!< table
		static const size_t   Result=16384; //!< 2*size
	};
}
#endif
