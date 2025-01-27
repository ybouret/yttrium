//! \file

#ifndef Y_Apex_Number_Included
#define Y_Apex_Number_Included 1

#include "y/apex/types.hpp"
#include "y/stream/serializable.hpp"
#include "y/type/identifiable.hpp"

namespace Yttrium
{
    namespace Apex
    {


        //! base class for numbers
        class Number : public Serializable, public Identifiable
        {
        protected:
            explicit Number() noexcept;
            
        public:
            virtual ~Number() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Number);
        };

    }

}

#endif
