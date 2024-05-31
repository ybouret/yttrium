//! \file

#ifndef Y_Kemp_Natural_Included
#define Y_Kemp_Natural_Included 1

#include "y/kemp/number.hpp"

namespace Yttrium
{
    namespace Kemp
    {
        class Element;

        class Natural : public Number
        {
        public:
            virtual ~Natural() noexcept;

            Natural();
            Natural(const Natural &);
            Natural &operator=(const Natural &);

            void xch(Natural &) noexcept;

            String toHex() const;


        private:
            mutable Element *code;
        };

    }
    typedef Kemp::Natural apn;
    
}

#endif

