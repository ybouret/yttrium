
//! \file

#ifndef Y_Apex_Natural_Included
#define Y_Apex_Natural_Included 1

#include "y/apex/number.hpp"

namespace Yttrium
{
    namespace Apex
    {


        class Natural : public Number
        {
        public:


            explicit Natural();
            virtual ~Natural() noexcept;


        private:

        };

    }

    typedef Apex::Natural APN;

}

#endif

