//! \file

#ifndef Y_Hashing_DES64_Included
#define Y_Hashing_DES64_Included 1

#include "y/hashing/mix64.hpp"

namespace Yttrium
{
    namespace Hashing
    {

        class DES64 : public Mix64
        {
        public:
            static const char * const CallSign;

            explicit DES64() noexcept;
            virtual ~DES64() noexcept;

            virtual const char * protocol()                         const noexcept;
            virtual void         operator()(uint32_t &, uint32_t &) const noexcept;

            bool test() const noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DES64);
        };

    }

}

#endif

