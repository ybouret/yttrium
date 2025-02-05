//! \file
#ifndef Y_DFT_Fmt8_Included
#define Y_DFT_Fmt8_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	struct DFT_Fmt8 {
		static const unsigned Count=2;     //!< count
		static const uint16_t Table[2][2]; //!< table
		static const size_t   Result=16; //!< 2*size
	};
}
#endif
