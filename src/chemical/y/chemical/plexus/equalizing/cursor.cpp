
#include "y/chemical/plexus/equalizing/cursor.hpp"
#include "y/chemical/plexus/equalizing/restartable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Equalizing
        {
            Cursor:: Cursor(const Cursor &_) : Hauler(_), xi(_.xi) {}


            Cursor:: ~Cursor() noexcept {}

            std::ostream & operator<<(std::ostream &os, const Cursor &cr)
            {
                os << std::setw(Restartable::Width) << cr.xi.str() << "@" << (const SRepo &)cr;
                return os;
            }

            Cursor:: Cursor(const SBank   &sb,
                            const Species &sp,
                            const xreal_t  xx) :
            Hauler(sb),
            xi(xx)
            {
                (*this) << sp;
            }

            xreal_t Cursor:: getXi() const noexcept
            {
                return xi;
            }
        }
        
    }

}


