
#include "y/system/wtime.hpp"
#include "y/system/exception.hpp"
#include "y/lockable.hpp"

#if defined(Y_Darwin)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(Y_Linux) || defined(Y_FreeBSD) || defined(Y_SunOS) || defined(Y_OpenBSD)
#define Y_USE_CLOCK_GETTIME 1
#include <time.h>
#include <sys/time.h>
#include <cerrno>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>

namespace Yttrium
{

#if defined(Y_Darwin)
    uint64_t WallTime::Ticks()
    {
        Y_GIANT_LOCK();
        return mach_absolute_time();
    }

    static inline double WallTimeCalibrate()
    {
        Y_GIANT_LOCK();
        mach_timebase_info_data_t timebase;
        const kern_return_t       err = mach_timebase_info(&timebase);
        if(KERN_SUCCESS !=err)
        {
            throw Mach::Exception(err,"mach_timebase_info");
        }
        const double  conversion_factor = double(timebase.numer) / timebase.denom;
        return 1e-9 * conversion_factor;
    }


#endif

#if defined(Y_USE_CLOCK_GETTIME)
    static const uint64_t __giga64 = Y_U64(0x3B9ACA00);

    uint64_t WallTime:: Ticks()
    {
        Y_GIANT_LOCK();
        struct timespec tp  = { 0, 0 };
        const int       err = clock_gettime( CLOCK_REALTIME, &tp );
        if(err!=0)
            throw Libc::Exception( errno, "clock_gettime" );
        std::cerr << tp.tc_sec << " s + " << tp.tv_nsec << " ns" << std::endl;
        return __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
    }


    static inline uint64_t WallTimeCalibrate()
    {
        Y_GIANT_LOCK();
        struct timespec tp  = { 0, 0 };
        const int       err = clock_getres(CLOCK_REALTIME,&tp);
        if(err!=0) throw Libc::Exception( errno, "clock_getres" );
        return  __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
    }


#endif

#if defined(Y_WIN)
    uint64_t WallTime::Ticks()
    {
        Y_GIANT_LOCK();
        int64_t Q = 0;
        if (!::QueryPerformanceCounter((LARGE_INTEGER*)&Q))
        {
            //throw win32::exception(::GetLastError(), " ::QueryPerformanceCounter");
        }
        return uint64_t(Q);
    }

    static inline double WallTimeCalibrate()
    {
        static const long double l_one = 1;
        Y_GIANT_LOCK();
        LARGE_INTEGER F;
        if( ! :: QueryPerformanceFrequency( &F ) )
        {
            throw Win32::Exception( ::GetLastError(), "::QueryPerformanceFrequency" );
        }
        return static_cast<double>(l_one / static_cast<long double>( F.QuadPart ));
    }


#endif

    WallTime:: ~WallTime() noexcept
    {
    }

    WallTime:: WallTime()
    {
        std::cerr << "calibrate: " << WallTimeCalibrate() << std::endl;
    }


}

