//! \file
#ifndef Y_DFT_Fmt32768_Included
#define Y_DFT_Fmt32768_Included
#include "y/config/starting.hpp"
namespace Yttrium {

	struct DFT_Fmt32768 {
		static const unsigned Count=16256;     //!< count
		static const uint16_t Table[16256][2]; //!< table
		static const size_t   Result=65536; //!< 2*size
	};
}
#endif
