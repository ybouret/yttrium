//! \file
#ifndef Y_DFT_Fmt64_Included
#define Y_DFT_Fmt64_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format64
	struct DFT_Fmt64 {
		static const unsigned Count=28;     //!< count
		static const uint16_t Table[28][2]; //!< table
		static const size_t   Result=128; //!< 2*size
	};
}
#endif
