#include "y/chemical/plexus/equalizer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const Clusters &clusters) :
        Ceqz( clusters.maxCPC, clusters.maxCPC > 0 ? clusters.maxSPC : 0)
        {
        }

    }

}

