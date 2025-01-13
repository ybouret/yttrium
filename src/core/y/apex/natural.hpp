
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/apex/types.hpp"
#include "y/ostream-proto.hpp"

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
            Y_OSTREAM_PROTO(Natural);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Natural);
            mutable Block *block;
        };

    }

}

#endif
