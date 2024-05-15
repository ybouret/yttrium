#include "y/chemical/plexus/equalizer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const Clusters &clusters) :
        Ceqz( clusters.maxCPC, clusters.maxSPC),
        banks(),
        fences(clusters.maxCPC,banks)
        {
        }

        void Equalizer:: tune(XWritable     &C0,
                              const Cluster &cluster)
        {

            const size_t m = cluster.species.size; assert(m>0); assert(m<=Ceqz.cols);

        }


    }

}

