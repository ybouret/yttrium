//! \file
#ifndef YACK_FUNCTOR_CALL_INCLUDED
#define YACK_FUNCTOR_CALL_INCLUDED 1


#include "y/object.hpp"
#include "y/type/traits.hpp"

namespace Yttrium
{

    namespace Kernel
    {

        //______________________________________________________________________
        //
        //
        //! callable interface construction
        //
        //______________________________________________________________________

#define YACK_CALLABLE_PROTOTYPE()                 \
/**/  private:                                    \
/**/   Y_DISABLE_COPY_AND_ASSIGN(callable);    \
/**/  public:                                     \
/**/    explicit callable() noexcept : Object() {} \
/**/    virtual ~callable() noexcept {}            \
/**/    virtual callable *clone() const = 0



#define Y_FUNCTOR_PARAM(TYPE) typename TypeTraits<TYPE>::ParamType

        //! memory management
#define YACK_CALLABLE_OBJECT() : public  Object
        //! defines the behavior of a generic callable object
        /**
         the parameters are send thru a typelist
         */
        template <typename R,class TLIST>
        class callable YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
        };

        //! no argument callable type
        template <typename R>
        class callable<R,NullType> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator() (void) = 0; //!< interface for no argument call
        };

        //! one argument callable type
        template <typename R, typename P1>
        class callable<R,TL1(P1)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()( Y_FUNCTOR_PARAM(P1) ) = 0;
        };

        //! two arguments callable type
        template <typename R, typename P1, typename P2>
        class callable<R,TL2(P1,P2)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()(Y_FUNCTOR_PARAM(P1),
                                 Y_FUNCTOR_PARAM(P2))= 0;
        };

        //! three arguments callable type
        template <typename R, typename P1, typename P2, typename P3>
        class callable<R,TL3(P1,P2,P3)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()(Y_FUNCTOR_PARAM(P1),
                                 Y_FUNCTOR_PARAM(P2),
                                 Y_FUNCTOR_PARAM(P3) )= 0;
        };

        //! four arguments callable type
        template <typename R, typename P1, typename P2, typename P3, typename P4>
        class callable<R,TL4(P1,P2,P3,P4)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()(Y_FUNCTOR_PARAM(P1),
                                 Y_FUNCTOR_PARAM(P2),
                                 Y_FUNCTOR_PARAM(P3),
                                 Y_FUNCTOR_PARAM(P4)
                                 ) = 0;
        };

        //! compute parameters aliases
#define Y_FUNCTOR_PARAMETERS() \
typedef typename TypeTraits< typename TL::SafeTypeAt<TLIST,0,EmptyType>::Result>::ParamType param1;\
typedef typename TypeTraits< typename TL::SafeTypeAt<TLIST,1,EmptyType>::Result>::ParamType param2;\
typedef typename TypeTraits< typename TL::SafeTypeAt<TLIST,2,EmptyType>::Result>::ParamType param3;\
typedef typename TypeTraits< typename TL::SafeTypeAt<TLIST,3,EmptyType>::Result>::ParamType param4


    }

}

#endif
