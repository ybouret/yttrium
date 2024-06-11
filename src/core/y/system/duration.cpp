
#include "y/system/duration.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/type/utils.hpp"
#include <iostream>
#include <cmath>
#include <cstring>


namespace Yttrium
{
    Duration:: ~Duration() noexcept
    {
        ldz();
    }

    Duration:: Duration(const double s) noexcept : wksp()
    {
        ld(s);
    }

    Duration:: Duration(const Duration &d) noexcept : wksp()
    {
        memcpy(wksp,d.wksp,sizeof(wksp));
    }

    Duration::Status & Duration:: getStatus() const noexcept
    {

        static const size_t NumWords = sizeof(wksp)/sizeof(wksp[0]);
        static const size_t LastWord = NumWords-1;
        return *static_cast<Status *>( Memory::OutOfReach::Addr( &wksp[LastWord]) );
    }

    void Duration:: ldz() noexcept
    {
        Y_STATIC_ZARR(wksp);
        getStatus() = Standard;
    }

    Duration::Status Duration::status() const noexcept
    {
        return getStatus();
    }


    void Duration:: ld(double s) noexcept
    {
        ldz();
        if(s<0)
        {
            getStatus() = Infinite;
        }
        else
        {
            s = Clamp<double>(0,s,MaxWait);
            struct timeval &out = getTV();
            const double    ns  = floor(s);
            const double    nu  = Clamp<double>(0,floor( 1e6 * (s-ns) + 0.5 ),999999);
            out.tv_sec  = static_cast<int>(ns);
            out.tv_usec = static_cast<int>(nu);
        }

    }

    Duration:: operator double() const noexcept
    {
        switch( status() )
        {
            case Infinite: return -1;
            case Standard: break;
        }
        const struct timeval &me = getTV();
        return Clamp<double>(0,double(me.tv_sec) + 1e-6 * double(me.tv_usec),MaxWait);
    }



    Duration & Duration:: operator=(const Duration &d) noexcept
    {
        memmove(wksp, d.wksp, sizeof(wksp) );
        return *this;
    }

    Duration & Duration:: operator=(const double s) noexcept
    {
        ld(s);
        return *this;
    }


    struct timeval * Duration:: tv() noexcept
    {
        switch( status() )
        {
            case Infinite: return 0;
            case Standard:
                break;
        }
        return & getTV();
    }

    const struct timeval * Duration:: tv() const noexcept
    {
        switch( status() )
        {
            case Infinite: return 0;
            case Standard:
                break;
        }
        return & getTV();
    }



    struct timeval & Duration:: getTV() const noexcept
    {
        return *static_cast<struct timeval *>( Memory::OutOfReach::Addr(wksp) );
    }


#if 0
    struct timeval * Duration:: tv() noexcept
    {
        return static_cast<struct timeval *>( Memory::OutOfReach::Addr(wksp) );
    }
#endif

}
