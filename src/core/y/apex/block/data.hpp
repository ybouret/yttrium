
//! \file

#ifndef Y_Apex_Block_Data_Included
#define Y_Apex_Block_Data_Included 1

#include "y/apex/block/jig/api.hpp"
#include "y/calculus/base2.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace Apex
    {
        //______________________________________________________________________
        //
        //
        //
        //! Memory for Block
        //
        //
        //______________________________________________________________________
        class DataBlock : public Quantized
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t      MinRange = 2 * sizeof(natural_t);          //!< alias
            static const unsigned    MinShift = iLog2<MinRange>::Value;         //!< alias
            static const unsigned    MaxShift = Base2<size_t>::MaxShift;        //!< alias
            static const size_t      MaxBytes = Base2<size_t>::MaxPowerOfTwo;   //!< alias
            static const size_t      One      = 1;                              //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit DataBlock(const unsigned _shift); //!< setup

        public:
            virtual ~DataBlock() noexcept;            //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void set(const size_t ibit) noexcept;       //!< iff plan=Plan1
            bool get(const size_t ibit) const noexcept; //!< iff plan=Plan1

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
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

