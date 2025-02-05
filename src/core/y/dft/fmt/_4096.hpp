//! \file
#ifndef Y_DFT_Fmt4096_Included
#define Y_DFT_Fmt4096_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	struct DFT_Fmt4096 {
		static const unsigned Count=2016;     //!< count
		static const uint16_t Table[2016][2]; //!< table
		static const size_t   Result=8192; //!< 2*size
	};
}
#endif
