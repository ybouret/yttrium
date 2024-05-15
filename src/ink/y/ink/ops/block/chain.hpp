//! \file

#ifndef Y_Ink_Block_Chain_Included
#define Y_Ink_Block_Chain_Included 1

#include "y/ink/pixmap.hpp"

namespace Yttrium
{
    namespace Ink
    {


        //! chaining two operators
        template <typename OP1, typename OP2>
        struct Chain2
        {
            //! call with a temporary field
            template <typename T> static inline
            void Call(Slabs           &slabs,
                      Pixmap<T>       &target,
                      Pixmap<T>       &middle,
                      const Pixmap<T> &source)
            {
                OP1::Call(slabs,middle,source);
                OP2::Call(slabs,target,middle);
            }
        };

        //! chaining three operators
        template <typename OP1, typename OP2, typename OP3>
        struct Chain3
        {
            //! call with a temporary field
            template <typename T> static inline
            void Call(Slabs           &slabs,
                      Pixmap<T>       &target,
                      Pixmap<T>       &middle,
                      const Pixmap<T> &source)
            {
                OP1::Call(slabs,target,source);
                OP2::Call(slabs,middle,target);
                OP3::Call(slabs,target,middle);
            }
        };

    }

}

#endif

