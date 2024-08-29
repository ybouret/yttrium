#include "y/chemical/plexus/warden/sproxy.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        SProxy::  SProxy(const SBank &sbank) noexcept : Proxy<const SRepo>(), sr(sbank) {}
        SProxy:: ~SProxy() noexcept {}
        SProxy::  SProxy(const SProxy &_) : Proxy<const SRepo>(), sr(_.sr) {}

        SProxy & SProxy:: operator<<(const SProxy &other)
        {
            sr << other.sr;
            return *this;
        }
    }

}

