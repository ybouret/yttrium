

//! \file
#ifndef Y_STATIC_CHECK_INCLUDED
#define Y_STATIC_CHECK_INCLUDED  1

namespace Yttrium
{
    //! compile_time_check for true values
    template <bool> struct CompileTimeCheck {
        CompileTimeCheck(...); //!< accept any argument
    };

    //! will failed since constructor is not defined
    template <> struct CompileTimeCheck<false> {};

    //! implement a local static check
#define Y_STATIC_CHECK(expr,msg)                 \
/*    */    {                                    \
/*    */        class ERROR_##msg {};            \
/*    */        (void)sizeof( (Yttrium::CompileTimeCheck<(expr)==true>(ERROR_##msg())) );\
/*    */    }

}

#endif
