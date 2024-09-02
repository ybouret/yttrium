
//! \file

#ifndef Y_Chemical_Joint_Included
#define Y_Chemical_Joint_Included 1

#include "y/chemical/plexus/cluster.hpp"
#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Joint : public Quantized, public Counted
        {
        public:
            typedef LittleEndianKey     KeyType;
            typedef LittleEndianAddress KeyAddr;

            explicit Joint(const Cluster &cluster) noexcept;
            virtual ~Joint() noexcept;

            const KeyType & key() const noexcept;

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

