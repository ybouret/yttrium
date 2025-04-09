
//! \file


#ifndef Y_Chemical_Equalizer_OneSided_Included
#define Y_Chemical_Equalizer_OneSided_Included 1


#include "y/chemical/plexus/cluster.hpp"
#include "y/chemical/plexus/equalizer/cursors.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            class OneSided
            {
            public:
                static const char * const CallSign;

                explicit OneSided(const Cluster &_cluster,
                                  const Banks   &_banks);
                virtual ~OneSided() noexcept;

                const Cluster &cluster;
                Cursors        cursors;

                void fix(XMLog &xml,
                         XWritable & C0,
                         const Level L0);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(OneSided);
            };

        }

    }

}

#endif
