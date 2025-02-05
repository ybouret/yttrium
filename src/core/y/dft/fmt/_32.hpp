//! \file
#ifndef Y_DFT_Fmt32_Included
#define Y_DFT_Fmt32_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format32
	struct DFT_Fmt32 {
		static const unsigned Count=12;     //!< count
		static const uint16_t Table[12][2]; //!< table
		static const size_t   Result=64; //!< 2*size
	};
}
#endif
