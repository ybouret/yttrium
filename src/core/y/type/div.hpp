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
            typedef div_t  Type;
            typedef int    Args;
            typedef Type (*Proc)(Args,Args);
            static const   Proc   Call;
        };

        struct DivLong
        {
            static const char * const CallSign;
            typedef ldiv_t Type;
            typedef long   Args;
            typedef Type (*Proc)(Args,Args);
            static const   Proc   Call;
        };

        struct DivLongLong
        {
            static const char * const CallSign;
            typedef lldiv_t   Type;
            typedef long long Args;
            typedef Type (*Proc)(Args,Args);
            static const   Proc   Call;
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

        typedef typename      API::Type Type;
        static const typename API::Proc Call;
    };

    template <typename T>
    const typename Div<T>::API::Proc Div<T>::Call = API::Call;

}

#endif

