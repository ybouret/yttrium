//! \file

#ifndef Y_Apex_Number_Included
#define Y_Apex_Number_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    namespace Apex
    {



        typedef uint64_t UIntType;

#define Y_APEX_ENCODE(Core,Word) ( ( (Core) << 8 ) | (Word) )

#if !defined(Y_APEX_METRICS)
#define Y_APEX_METRICS Y_APEX_ENCODE(0,0)
#endif


        //______________________________________________________________________
        //
        //
        //
        //! base class for any Apex number
        //
        //
        //______________________________________________________________________
        class Number
        {

        public:

#if Y_APEX_ENCODE(0,0) == Y_APEX_METRICS
            //! default
            typedef UnsignedInt<sizeof(void*)>::Type CoreType;
            static const unsigned                    CoreSize = sizeof(CoreType);
            static const unsigned                    WordSize = CoreSize/2;
            typedef UnsignedInt<WordSize>::Type      WordType;
#endif

        protected:
            explicit Number() noexcept;
            
        public:
            virtual ~Number() noexcept; //!< cleanup


        private:
            Y_DISABLE_ASSIGN(Number);
        };
    }
}

#endif

