
//! \file

#ifndef Y_Chemical_Joint_Included
#define Y_Chemical_Joint_Included 1

#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for shared object with cluster's address as key
        //
        //
        //______________________________________________________________________
        class Joint : public Quantized, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Joint(const Cluster &cluster) noexcept; //!< setup
            virtual ~Joint() noexcept;                       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const KeyType & key() const noexcept; //!< key_

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Cluster &mine; //!< persistent cluster
            const size_t   neqs; //!< mine.size
            const size_t   nspc; //!< mine.species.size
            const KeyAddr  key_; //!< using mine

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Joint);
        };
    }

}

#endif

