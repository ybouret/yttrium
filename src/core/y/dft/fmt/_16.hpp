//! \file
#ifndef Y_DFT_Fmt16_Included
#define Y_DFT_Fmt16_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	struct DFT_Fmt16 {
		static const unsigned Count=6;     //!< count
		static const uint16_t Table[6][2]; //!< table
		static const size_t   Result=32; //!< 2*size
	};
}
#endif
