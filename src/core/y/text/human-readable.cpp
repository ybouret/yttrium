
#include "y/text/human-readable.hpp"


namespace Yttrium
{

    const char HumanReadable:: Suffix[] =
    {
        'b', //! 10^0
        'k', //! 10^3
        'M'  //! 10^6
    };

    static const size_t         ExpStep = 3;
    const size_t HumanReadable::Number  = sizeof(HumanReadable::Suffix) / sizeof(HumanReadable::Suffix[0]);
    const size_t HumanReadable::MaxExp  = ExpStep*(Number-1);

}
