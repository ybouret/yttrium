//! \file

#ifndef Y_Type_Div_Included
#define Y_Type_Div_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    namespace Core
    {
        struct DivInt
        {
            static const char * const CallSign;
            typedef div_t Type;
        };

        struct DivLong
        {
            static const char * const CallSign;
            typedef ldiv_t Type;
        };

        struct DivLongLong
        {
            static const char * const CallSign;
            typedef ldiv_t Type;
        };
    }

    template <typename T>
    struct Div
    {
        static const size_t TypeSize      = sizeof(T);
        static const bool   ChooseInt     = TypeSize <= sizeof(int);
        static const bool   ChooseLong    = TypeSize <= sizeof(long);


        typedef typename Pick<ChooseInt,
        Core::DivInt,
        typename Pick<ChooseLong,Core::DivLong,Core::DivLongLong>::Type >::Type DivType;

    };

}

#endif

