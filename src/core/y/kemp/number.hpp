//! \file

#ifndef Y_Kemp_Number_Included
#define Y_Kemp_Number_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //______________________________________________________________________
        //
        //
        //
        //! base class for Natural/Integer/Rational
        //
        //
        //______________________________________________________________________
        class Number
        {
        public:
            virtual ~Number() noexcept;

        protected:
            explicit Number() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Number);
        };

    }
}

#endif

