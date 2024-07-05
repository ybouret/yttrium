//! \file

#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Species : public Entity
        {
        public:
            template <typename ID> inline
            explicit Species(const ID &id,
                             const int zz) : Entity(id), z(zz)
            {
            }

            virtual ~Species() noexcept;
            Y_OSTREAM_PROTO(Species);


            const int z;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };
    }
}

#endif

