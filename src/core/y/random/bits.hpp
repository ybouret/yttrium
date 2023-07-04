// \file

#ifndef Y_Random_Bits_Included
#define Y_Random_Bits_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Random
    {

        

        class Bits
        {
        protected:
            explicit Bits(const uint32_t maxU32) noexcept;

            const uint32_t    umax;
            

        public:
            virtual ~Bits() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bits);
        };


    }

}

#endif

