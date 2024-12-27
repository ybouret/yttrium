
#include "y/chemical/plexus/boundary.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Boundary:: ~Boundary() noexcept {}

        Boundary:: Boundary(const Boundary &_) noexcept :
        sp(_.sp),
        xx(_.xx)
        {

        }

        Boundary:: Boundary(const Species &_sp, const xReal _xx) noexcept :
        sp(_sp),
        xx(_xx)
        {
        }
        


        const String & Boundary:: key() const noexcept
        {
            return sp.key();
        }

        const xReal & Boundary:: operator*() const noexcept
        {
            return xx;
        }

        xReal & Boundary:: operator*() noexcept
        {
            return xx;
        }
    }

}
