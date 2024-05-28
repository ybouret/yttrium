
//! \file

#ifndef Y_APK_Natural_Included
#define Y_APK_Natural_Included 1

#include "y/apk/number.hpp"

namespace Yttrium
{
    namespace APK
    {


        class Natural : public Number
        {
        public:
            virtual ~Natural() noexcept;
            Natural();
            Natural(const Natural &other);
            Natural(const uint64_t);
            Natural &operator=(const Natural &);
            

            String toHex() const;
            
        private:
            class Code;
            mutable Code *code;
        };

    }

}

#endif
