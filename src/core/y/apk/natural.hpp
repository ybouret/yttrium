
//! \file

#ifndef Y_APK_Natural_Included
#define Y_APK_Natural_Included 1

#include "y/apk/number.hpp"

namespace Yttrium
{
    namespace APK
    {

        class Component;

        class Natural : public Number
        {
        public:
            explicit Natural();
            virtual ~Natural() noexcept;
            
        private:
            Component *cm;
        };

    }

}

#endif
