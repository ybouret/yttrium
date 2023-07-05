// \file

#ifndef Y_Random_Bits_Included
#define Y_Random_Bits_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Random
    {
        template <typename T> class Metrics32;
        
        
        //! Random Bits interface
        class Bits
        {
        public:
            explicit Bits(const uint32_t maxU32) noexcept;
            virtual ~Bits() noexcept;

            const uint32_t                 umax;
            const Metrics32<float>       * const metricsF;
            const Metrics32<double>      * const metricsD;
            const Metrics32<long double> * const metricsL;

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Bits);
        };


    }

}

#endif

