//! \file
#ifndef Y_DFT_Fmt1024_Included
#define Y_DFT_Fmt1024_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	//! Format1024
	struct DFT_Fmt1024 {
		static const unsigned Count=496;     //!< count
		static const uint16_t Table[496][2]; //!< table
		static const size_t   Result=2048; //!< 2*size
	};
}
#endif
