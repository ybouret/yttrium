//! \file

#ifndef Y_Chemical_Conservations_Included
#define Y_Chemical_Conservations_Included 1

#include "y/chem/plexus/conservation.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Conservations : public Conservation::List
        {
        public:
            explicit Conservations() noexcept;
            virtual ~Conservations() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Conservations);
        };

    }

}

#endif
