
#include "y/chemical/plexus/type/marker.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Marker:: Marker(const SBank & _) noexcept :
        Proxy<const SRepo>(),
        srepo(_),
        value(0)
        {}

        Marker:: Marker(const Marker &_) :
        Proxy<const SRepo>(),
        srepo(_.srepo),
        value(_.value) {}

        Marker:: ~Marker() noexcept
        {
        }

        Y_PROXY_IMPL(Marker,srepo)

        std::ostream & operator<<(std::ostream &os, const Marker &mk)
        {
            os << mk.srepo << "=" << mk.value.str();
            return os;
        }

       
    }

}

