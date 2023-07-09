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
            typedef int   Args;
            static  Type (*Proc)(Args,Args);

        };

        struct DivLong
        {
            static const char * const CallSign;
            typedef ldiv_t Type;
            typedef long   Args;
            static  Type (*Proc)(Args,Args);
        };

        struct DivLongLong
        {
            static const char * const CallSign;
            typedef lldiv_t   Type;
            typedef long long Args;
            static  Type    (*Proc)(Args,Args);
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

