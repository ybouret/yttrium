//! \file


#ifndef Y_Apex_Number_Included
#define Y_Apex_Number_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Apex
    {
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
            virtual ~Number() noexcept; //!< cleanup

        private:
            Y_DISABLE_ASSIGN(Number);
        };
    }
}

#endif

