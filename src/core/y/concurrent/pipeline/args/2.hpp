
//! \file

#ifndef Y_Concurrent_Pipeline_ARGS2_Included
#define Y_Concurrent_Pipeline_ARGS2_Included 1

#include "y/type/traits.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        template <class ARG1, class ARG2>
        class TwoArguments
        {
        public:
            inline explicit TwoArguments(typename TypeTraits<ARG1>::ParamType a1,
                                  typename TypeTraits<ARG2>::ParamType a2) :
            arg1(a1),
            arg2(a2)
            {

            }

            inline virtual ~TwoArguments() noexcept
            {

            }

        protected:
            ARG1 arg1;
            ARG2 arg2;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(TwoArguments);
        };

    }
}
#endif

