//! \file

#ifndef Y_Type_Div_Included
#define Y_Type_Div_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    namespace Core
    {
        //! using  div_t/div
        class DivInt
        {
        public:
            static const char * const CallSign; //!< "DivInt"
            typedef div_t  Type;                //!< alias
            typedef int    Args;                //!< alias
            typedef Type (*Proc)(Args,Args);    //!< alias
            
            explicit DivInt() noexcept; //!< load call
            virtual ~DivInt() noexcept; //!< cleanup

            Proc const call; //!< div

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DivInt);
        };

        //! using ldiv_t/ldiv
        class DivLong
        {
        public:
            static const char * const CallSign; //!< "DivLong"
            typedef ldiv_t Type;                //!< alias
            typedef long   Args;                //!< alias
            typedef Type (*Proc)(Args,Args);    //!< alias

            explicit DivLong() noexcept; //!< load call
            virtual ~DivLong() noexcept; //!< cleanup

            Proc const call;            //!< ldiv

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DivLong);
        };

        //! using lldiv_t/lldiv
        class DivLongLong
        {
        public:
            static const char * const CallSign; //!< "DivLongLong"
            typedef lldiv_t   Type;             //!< alias
            typedef long long Args;             //!< alias
            typedef Type (*Proc)(Args,Args);    //!< alias

            explicit DivLongLong() noexcept;    //!< load call
            virtual ~DivLongLong() noexcept;    //!< cleanup

            Proc const call;                    //!< lldiv
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DivLongLong);
        };

        //! Div API Selector
        template <typename T> struct DivSelect
        {
            static const bool   ChooseInt     = sizeof(T) <= sizeof(int);  //!< alias
            static const bool   ChooseLong    = sizeof(T) <= sizeof(long); //!< alias

            //! optimized API computation
            typedef typename Pick<ChooseInt,
            Core::DivInt,
            typename Pick<ChooseLong,Core::DivLong,Core::DivLongLong>::Type >::Type API;
        };

    }


    //__________________________________________________________________________
    //
    //
    //! selecting and using the proper div_t/div
    //
    //__________________________________________________________________________
    template <typename T>
    class Div : public Core::DivSelect<T>::API
    {
    public:
        inline explicit Div() noexcept : Core::DivSelect<T>::API() {} //!< setup
        inline virtual ~Div() noexcept {}                             //!< cleanup
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Div);
    };


}

#endif

