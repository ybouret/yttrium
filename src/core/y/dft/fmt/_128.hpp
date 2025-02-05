//! \file
#ifndef Y_DFT_Fmt128_Included
#define Y_DFT_Fmt128_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format128
	struct DFT_Fmt128 {
		static const unsigned Count=56;     //!< count
		static const uint16_t Table[56][2]; //!< table
		static const size_t   Result=256; //!< 2*size
	};
}
#endif
