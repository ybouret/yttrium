
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class Range
        {
        public:
            inline  Range() noexcept;
            inline ~Range() noexcept;

            const size_t length;
            const size_t offset;
        private:
            Y_DISABLE_ASSIGN(Range);
        };

        class SIMD : public Readable<const Range>, public Range
        {
        public:
            static const char * const CallSign;

            explicit SIMD(const SharedLoop &);
            explicit SIMD(Loop             *);
            virtual ~SIMD() noexcept;

            size_t dispatch(const size_t dataLength, const size_t dataOffset=1) noexcept;


            virtual size_t       size() const noexcept;
            virtual const char * callSign() const noexcept;
            virtual const Range &operator[](const size_t) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            class Code;
            SharedLoop  loop;
            Code       *code;

        };

    }

}

#endif
