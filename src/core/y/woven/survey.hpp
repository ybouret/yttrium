
//! \file

#ifndef Y_WOVEn_Survey_Included
#define Y_WOVEn_Survey_Included 1

#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //______________________________________________________________________
        //
        //
        //! base class for a Survey
        //
        //______________________________________________________________________
        class Survey
        {
        public:
            virtual ~Survey() noexcept;        //!< cleanup

        protected:
            explicit Survey(XMLog &) noexcept; //!< setip

        public:
            XMLog &xml; //!< output helper

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Survey);
        };
    }

}

#endif
