
#include "y/chemical/plexus/conservation/broken.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            Broken:: Broken(const xreal_t &_xs,
                            const Law     &_law) noexcept :
            xs(_xs),
            law(_law)
            {
            }


            Broken:: Broken(const Broken &_) noexcept :
            xs(_.xs),
            law(_.law)
            {
            }

            Broken:: ~Broken() noexcept {}

            std::ostream & Broken:: show(std::ostream &os, const Assembly &fmt) const
            {
                fmt.pad(os << law.name, law) << " @" << xs.str();
                return os;
            }
        }

    }

}
