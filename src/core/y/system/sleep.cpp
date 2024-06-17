#include "y/system/sleep.hpp"
#include "y/type/utils.hpp"

#if defined(Y_BSD)
#include <time.h>
#endif

#if defined(Y_WIN)
#include <windows.h>
#endif

namespace Yttrium
{
	void SystemSleep::For(unsigned ms)
	{
		ms = Min(ms, MaxMilliSeconds);

#if defined(Y_WIN)
		::Sleep(ms);
#endif

#if defined(Y_BSD)
		const unsigned quot_s = ms / 1000;
		const unsigned rem_ms = (ms - (quot_s * 1000));
		const unsigned cnv_ns = rem_ms * 1000000;
		struct timespec period = { quot_s, cnv_ns };
		(void)nanosleep(&period, 0);
#endif

	}

}
