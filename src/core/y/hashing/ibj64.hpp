
//! \file

#ifndef Y_Hashing_IBJ64_Included
#define Y_Hashing_IBJ64_Included 1

#include "y/hashing/mix64.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        //! integer hashing
        uint32_t IBJ32(uint32_t a) noexcept;

        class IBJ64 : public Mix64
        {
        public:
            static const char * const CallSign;

            explicit IBJ64() noexcept;
            virtual ~IBJ64() noexcept;

            virtual const char * protocol()                         const noexcept;
            virtual void         operator()(uint32_t &, uint32_t &) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IBJ64);
        };

    }

}

#endif

