
//! \file

#ifndef Y_Concurrent_Default_Topology_Included
#define Y_Concurrent_Default_Topology_Included 1

#include "y/concurrent/topology.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Topology from Y_NUM_THREADS
        //
        //
        //______________________________________________________________________
        class DefaultTopology : public Object, public Topology
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Yttrium::AutoPtr<const DefaultTopology> Handle; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit DefaultTopology();           //!< make a topology
            virtual ~DefaultTopology() noexcept;  //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DefaultTopology);
        };

    }

}

#endif
