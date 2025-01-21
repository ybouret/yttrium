
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"
#include "y/apex/block/ptr.hpp"
#include "y/apex/block/mtx.hpp"
#include "y/ostream-proto.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class Block;

        enum AddOps
        {
            Add8_1,
            Add8_2,
            Add8_4,

            Add4_1,
            Add4_2,

            Add2_1
        };

        class Natural : public Number
        {
        public:
            static const char * const CallSign;
            static const unsigned     NumAddOps = Add2_1+1;
            static const AddOps       AddOpsTable[NumAddOps];

            class AutoLock {
            public:
                AutoLock(const Natural &) noexcept;
                ~AutoLock()               noexcept;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(AutoLock);
                const Natural &host;
            };


            Natural(const natural_t);
            Natural(Random::Bits &, const size_t);
            virtual ~Natural() noexcept;
            Natural(const Natural &);
            Natural & operator=(const Natural &);
            Natural & operator=(const natural_t)  noexcept;
            Y_OSTREAM_PROTO(Natural);

            Lockable & operator*()  const noexcept;
            Lockable * operator->() const noexcept;


            Block *Add(Block &lhs, Block &rhs, const AddOps addOps, uint64_t * const ell);

            Block &       _block() noexcept;
            const Block & _block() const noexcept;

        private:
            mutable BlockPtr block;
            mutable MutexPtr mutex;
        };

    }

}

#endif
