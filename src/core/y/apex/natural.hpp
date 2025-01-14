
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/apex/block/ptr.hpp"
#include "y/apex/block/mtx.hpp"
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

            Lockable & operator*() const noexcept;

        private:
            mutable BlockPtr block;
            mutable MutexPtr mutex;
        };

    }

}

#endif
