
//! \file

#ifndef Y_Chemical_Devices_Included
#define Y_Chemical_Devices_Included 1

#include "y/chemical/plexus/device.hpp"
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Devices : public Device::Set
        {
        public:
            explicit Devices(const Clusters &, const bool doTrace=false);
            virtual ~Devices() noexcept;

            void    operator()(XWritable       &C,
                               const Level      L,
                               const XReadable &K,
                               XMLog           &xml);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Devices);
        };
    }

}

#endif

