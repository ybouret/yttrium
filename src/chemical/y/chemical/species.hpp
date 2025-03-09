
//! \file


#ifndef Y_Chemical_Species_Included
#define Y_Chemical_Species_Included 1

#include "y/chemical/type/entity.hpp"
#include "y/chemical/formula.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Species : public Formula, public Entity
        {
        public:
            virtual ~Species() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Species);
        };

    }

}

#endif

