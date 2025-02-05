//! \file
#ifndef Y_DFT_Fmt16384_Included
#define Y_DFT_Fmt16384_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format16384
	struct DFT_Fmt16384 {
		static const unsigned Count=8128;     //!< count
		static const uint16_t Table[8128][2]; //!< table
		static const size_t   Result=32768; //!< 2*size
	};
}
#endif
