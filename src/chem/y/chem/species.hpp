//! \file


#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chem/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Species : public Entity
        {
        public:
            template <typename NAME> inline
            explicit Species(const NAME &uid,
                             const int   algebraicCharge) :
            Entity(uid), z(algebraicCharge) {}
            virtual ~Species() noexcept;


            const int z;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
}

#endif

