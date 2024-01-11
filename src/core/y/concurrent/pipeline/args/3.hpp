

//! \file

#ifndef Y_Concurrent_Pipeline_ARGS3_Included
#define Y_Concurrent_Pipeline_ARGS3_Included 1

#include "y/type/traits.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //! holding two arguments for task
        //
        //______________________________________________________________________
        template <class ARG1, class ARG2, class ARG3>
        class ThreeArguments
        {
        public:

            //! setup
            inline explicit ThreeArguments(typename TypeTraits<ARG1>::ParamType a1,
                                           typename TypeTraits<ARG2>::ParamType a2,
                                           typename TypeTraits<ARG3>::ParamType a3) :
            arg1(a1),
            arg2(a2),
            arg3(a3)
            {
            }

            //! cleanup
            inline virtual ~ThreeArguments() noexcept
            {
            }

        protected:
            ARG1 arg1; //!< arg1
            ARG2 arg2; //!< arg2
            ARG3 arg3; //!< arg3

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ThreeArguments);
        };

    }
}
#endif

