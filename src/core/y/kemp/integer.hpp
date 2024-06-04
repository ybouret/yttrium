
//! \file

#ifndef Y_Kemp_Integer_Included
#define Y_Kemp_Integer_Included 1

#include "y/kemp/natural.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        class Integer : public Number
        {
        public:
            Integer();
            Integer(const int64_t);
            Integer(const Integer &);
            Integer(const SignType, const Natural &);
            virtual ~Integer() noexcept;
            Integer & operator=(const Integer &z);
            Integer & operator=(const int64_t  z);
            Y_OSTREAM_PROTO(Integer);

            void xch(Integer &z) noexcept;

            const SignType s;
            const Natural  n;
        };

    }

}

#endif

