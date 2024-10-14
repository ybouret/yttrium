
//! \file

#ifndef Y_Chemical_Devices_Included
#define Y_Chemical_Devices_Included 1

#include "y/chemical/plexus/device.hpp"
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Device to solve all pre-computed clusters
        //
        //
        //______________________________________________________________________
        class Devices : public Device::Set
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Devices(const Clusters &, const bool doTrace=false); //!< setup
            virtual ~Devices() noexcept;                                  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compute all phase spaces
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

