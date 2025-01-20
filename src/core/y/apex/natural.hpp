
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

            class AutoLock {
            public:
                AutoLock(const Natural &) noexcept;
                ~AutoLock()               noexcept;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(AutoLock);
                const Natural &host;
            };


            Natural(const natural_t);
            virtual ~Natural() noexcept;
            Natural(const Natural &);
            Natural & operator=(const Natural &);
            Natural & operator=(const natural_t)  noexcept;
            Y_OSTREAM_PROTO(Natural);

            Lockable & operator*()  const noexcept;
            Lockable * operator->() const noexcept;

        private:
            mutable BlockPtr block;
            mutable MutexPtr mutex;
        };

    }

}

#endif
