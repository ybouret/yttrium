
//! \file

#ifndef Y_APK_Number_Included
#define Y_APK_Number_Included 1

#include "y/object.hpp"

namespace Yttrium
{
    namespace APK
    {

        class Number : public Object
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

