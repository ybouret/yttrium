
//! \file

#ifndef Y_WOVEn_Survey_Included
#define Y_WOVEn_Survey_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        class Survey
        {
        public:
            virtual ~Survey() noexcept;

        protected:
            explicit Survey(const bool) noexcept;

        public:
            bool verbose;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Survey);
        };
    }

}

#endif
