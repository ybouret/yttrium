
//! \file

#ifndef Y_Apex_Block_Data_Included
#define Y_Apex_Block_Data_Included 1

#include "y/apex/jig/api.hpp"
#include "y/calculus/base2.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Apex
    {
        class DataBlock : public Quantized
        {
        public:
            static const size_t      MinRange = 2 * sizeof(natural_t);
            static const unsigned    MinShift = iLog2<MinRange>::Value;
            static const unsigned    MaxShift = Base2<size_t>::MaxShift;
            static const size_t      MaxBytes = Base2<size_t>::MaxPowerOfTwo;
            static const size_t      One      = 1;

        protected:
            explicit DataBlock(const unsigned _shift);

        public:
            virtual ~DataBlock() noexcept;

            const unsigned shift;               //!< log2(range)
            void * const   entry;               //!< memory
            const size_t   bits;                //!< current number of bits
            const Plan     plan;                //!< current plan
            JigAPI * const curr;                //!< current API
            JigAPI *       dull[JigAPI::Faded]; //!< other dull API
            const size_t   range;               //!< 2^shift

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DataBlock);
        };
    }

}

#endif

