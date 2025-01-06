
#include "y/chemical/plexus/type/marker.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Marker:: Marker(const SBank & _) noexcept :
        srepo(_),
        value(0)
        {}

        Marker:: Marker(const Marker &_) :
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

        void Marker:: join(const Species &s)
        {
            assert( !srepo.has(s) );
            srepo << s;
        }

    }

}

