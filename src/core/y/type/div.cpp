
#include "y/type/div.hpp"

namespace Yttrium
{
	namespace Core
	{
		const char * const DivInt::CallSign = "DivInt";
		const char * const DivLong::CallSign = "DivLong";
		const char * const DivLongLong::CallSign = "DivLongLong";

		DivInt::Proc     const DivInt::Call = div;
		DivLong::Proc     const DivLong::Call = ldiv;
		DivLongLong::Proc const DivLongLong::Call = lldiv;

	}
}
