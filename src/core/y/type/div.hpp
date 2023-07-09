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
        static const bool   ChooseInt     = sizeof(T) <= sizeof(int);
        static const bool   ChooseLong    = sizeof(T) <= sizeof(long);


        typedef typename Pick<ChooseInt,
        Core::DivInt,
        typename Pick<ChooseLong,Core::DivLong,Core::DivLongLong>::Type >::Type API;

        typedef typename API::Type Type;


    };

}

#endif

