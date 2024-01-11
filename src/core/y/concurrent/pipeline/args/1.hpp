//! \file

#ifndef Y_Concurrent_Pipeline_ARGS1_Included
#define Y_Concurrent_Pipeline_ARGS1_Included 1

#include "y/type/traits.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        template <class ARG1>
        class OneArgument
        {
        public:
            inline explicit OneArgument(typename TypeTraits<ARG1>::ParamType a1) :
            arg1(a1)
            {
                
            }

            inline virtual ~OneArgument() noexcept
            {

            }

        protected:
            ARG1 arg1;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OneArgument);
        };

    }
}
#endif

