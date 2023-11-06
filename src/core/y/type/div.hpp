//! \file

#ifndef Y_Type_Div_Included
#define Y_Type_Div_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    namespace Core
    {
        //! using  div_t/div
        struct DivInt
        {
            static const char * const CallSign; //!< "DivInt"
            typedef div_t  Type;                //!< alias
            typedef int    Args;                //!< alias
            typedef Type (*Proc)(Args,Args);    //!< alias
            static const   Proc   Call;         //!< alias
        };

        //! using ldiv_t/ldiv
        struct DivLong
        {
            static const char * const CallSign; //!< "DivLong"
            typedef ldiv_t Type;                //!< alias
            typedef long   Args;                //!< alias
            typedef Type (*Proc)(Args,Args);    //!< alias
            static const   Proc   Call;         //!< alias
        };

        //! using lldiv_t/lldiv
        struct DivLongLong
        {
            static const char * const CallSign; //!< "DivLongLong"
            typedef lldiv_t   Type;             //!< alias
            typedef long long Args;             //!< alias
            typedef Type (*Proc)(Args,Args);    //!< alias
            static const   Proc   Call;         //!< alias
        };
    }


    //__________________________________________________________________________
    //
    //
    //! selecting and using the proper div_t/div
    //
    //__________________________________________________________________________
    template <typename T>
    struct Div
    {
        static const bool   ChooseInt     = sizeof(T) <= sizeof(int);  //!< alias
        static const bool   ChooseLong    = sizeof(T) <= sizeof(long); //!< alias

        //! optimized API computation
        typedef typename Pick<ChooseInt,
        Core::DivInt,
        typename Pick<ChooseLong,Core::DivLong,Core::DivLongLong>::Type >::Type API;

        typedef typename      API::Type Type; //!< alias
        static const typename API::Proc Call; //!< alias
    };

    //! instance
    template <typename T>
    const typename Div<T>::API::Proc Div<T>::Call = API::Call;

}

#endif

