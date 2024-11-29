
#include "y/chemical/species.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Species:: ~Species() noexcept
        {

        }


        const char * const Species:: CallSign = "Chemical::Species";
        const char * const Species:: Scheme   = "dark28";

        const String & Species:: key() const noexcept
        {
            return name;
        }

        String Species:: makeColor() const
        {
            return makeColorFrom(Scheme);
        }


    }

}
