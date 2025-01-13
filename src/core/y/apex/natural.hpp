
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/apex/types.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class Block;

        class Natural
        {
        public:
            Natural(const natural_t);
            virtual ~Natural() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Natural);
            Block *block;
        };

    }

}

#endif
