//! \file

#ifndef Y_Chemical_Indexed_Included
#define Y_Chemical_Indexed_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        enum Level
        {
            TopLevel,
            SubLevel,
            AuxLevel
        };

        class Indexed : public Entity
        {
        public:
            static const size_t Levels = AuxLevel + 1;


            explicit Indexed(const size_t topIndex) noexcept; //!< topIndex>0
            virtual ~Indexed()                      noexcept; //!< cleanup



            const size_t indx[Levels];

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
        };
    }

}

#endif

