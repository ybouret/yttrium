
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

        class Natural : public Number
        {
        public:
            Natural(const natural_t);
            virtual ~Natural() noexcept;
            Natural(const Natural &);
            Natural & operator=(const Natural &);
            Y_OSTREAM_PROTO(Natural);
            
        private:
            mutable Block *block;
        };

    }

}

#endif
