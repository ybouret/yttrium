#include "y/system/error.hpp"
#include "y/type/utils.hpp"

#include <cstring>
#include <cstdarg>
#include <cstdio>



namespace Yttrium
{

	namespace Libc
	{
		void FormatError(char* buffer,
			const size_t buflen,
			const int    err) noexcept
		{
			assert(Good(buffer, buflen));
			if (buflen > 1)
			{
				memset(buffer, 0, buflen);
				const char* errbuf = strerror(err);
				const size_t errlen = strlen(errbuf);
				memcpy(buffer, errbuf, Min(errlen, buflen - 1));
			}
		}

		void CriticalError(const int err, const char* fmt, ...)
		{
			static const char pfx[] = " *** ";
			assert(NULL != fmt);

			fputs(pfx, stderr);
			{
				va_list ap;
				va_start(ap, fmt);
				vfprintf(stderr, fmt, ap);
				va_end(ap);
			}
			fputc('\n', stderr);

			fputs(pfx, stderr);
			char buffer[256];
			Libc::FormatError(buffer, sizeof(buffer), err);
			fputs(buffer, stderr);
			fputc('\n', stderr);

			abort();
		}
	}


}

#if defined(Y_WIN)
#include <windows.h>

namespace Yttrium
{
	namespace Win32
	{
		//______________________________________________________________________
	   //
	   //
	   //! using strerror
	   //
	   //______________________________________________________________________
		void FormatError(
			char  *        buffer,
			const size_t   buflen,
			const uint32_t err) noexcept
		{
			assert(Good(buffer, buflen));
			memset(buffer, 0, buflen);
			if (buflen > 0)
			{
				const DWORD dw = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,  /* (not used with FORMAT_MESSAGE_FROM_SYSTEM) */
					err,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					buffer,
					(DWORD)buflen,
					NULL);
				if (dw <= 0)
				{
					yack_cstring_msgcpy(buffer, buflen, Core::Failure);
				}
				else
				{
					yack_cstring_trim(buffer, bad, sizeof(bad));
				}
			}
		}
	}

}

#endif
