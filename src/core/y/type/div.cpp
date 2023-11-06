
#include "y/type/div.hpp"

namespace Yttrium
{
	namespace Core
	{
		const char * const DivInt::CallSign = "DivInt";
		const char * const DivLong::CallSign = "DivLong";
		const char * const DivLongLong::CallSign = "DivLongLong";

        DivInt::  DivInt() noexcept : call(div) {}
        DivInt:: ~DivInt() noexcept {}



        DivLong::  DivLong() noexcept : call(ldiv) {}
        DivLong:: ~DivLong() noexcept {}

        
        DivLongLong::  DivLongLong() noexcept : call(lldiv) {}
        DivLongLong:: ~DivLongLong() noexcept {}

	}
}
