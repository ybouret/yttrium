
#include "y/chemical/plexus/equalizing/two-sided/gain.hpp"
#include "y/chemical/plexus/equalizing/restartable.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {
            Gain:: Gain(const xreal_t      _g,
                 const Components & _E,
                 const XReadable  & _c) noexcept :
            g(_g), E(_E), C(_c)
            {
            }

            Gain:: ~Gain() noexcept {}

            Gain:: Gain(const Gain &_) noexcept :
            g(_.g), E(_.E), C(_.C)
            {
            }

            std::ostream & operator<<(std::ostream &os, const Gain &gain)
            {
                os << std::setw(Restartable::Width) << gain.g.str() << " @" << gain.E.name << "=";
                gain.E.displayCompact(os,gain.C,SubLevel);
                return os;
            }

        }

    }

}


