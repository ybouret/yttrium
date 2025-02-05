//! \file
#ifndef Y_DFT_Fmt2048_Included
#define Y_DFT_Fmt2048_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format2048
	struct DFT_Fmt2048 {
		static const unsigned Count=992;     //!< count
		static const uint16_t Table[992][2]; //!< table
		static const size_t   Result=4096; //!< 2*size
	};
}
#endif
