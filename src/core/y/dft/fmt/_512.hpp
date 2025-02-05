//! \file
#ifndef Y_DFT_Fmt512_Included
#define Y_DFT_Fmt512_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format512
	struct DFT_Fmt512 {
		static const unsigned Count=240;     //!< count
		static const uint16_t Table[240][2]; //!< table
		static const size_t   Result=1024; //!< 2*size
	};
}
#endif
