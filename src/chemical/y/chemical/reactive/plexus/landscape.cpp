#include "y/chemical/reactive/plexus/landscape.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Landscape:: ~Landscape() noexcept {}
        
        Landscape:: Landscape(const BBank &bbank,
                              const SBank &sbank) noexcept :
        capping(bbank,sbank),
        missing(bbank,sbank)
        {
        }

        Landscape:: Landscape(const Landscape &other) : capping(other.capping), missing(other.missing)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Landscape &self)
        {
            os << "capping=" << self.capping << " / missing=" << self.missing;
            return os;
        }

    }

}

