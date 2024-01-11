

//! \file

#ifndef Y_Concurrent_Command3_Included
#define Y_Concurrent_Command3_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/args/3.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //! encapsulate call to OBJECT.METHOD call
        //
        //______________________________________________________________________
        template <typename OBJECT, typename METHOD, typename ARG1, typename ARG2,typename ARG3>
        class Command3 : public Runnable, public ThreeArguments<ARG1,ARG2,ARG3>
        {
        public:
            //! setup
            inline explicit Command3(OBJECT                              &o,
                                     METHOD                               m,
                                     typename TypeTraits<ARG1>::ParamType a1,
                                     typename TypeTraits<ARG2>::ParamType a2,
                                     typename TypeTraits<ARG3>::ParamType a3) noexcept :
            Runnable(),
            host(o),
            meth(m),
            ThreeArguments<ARG1,ARG2,ARG3>(a1,a2,a3)
            {}

            //! cleanup
            inline virtual ~Command3() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command3);
            inline virtual void run(const ThreadContext &context) { (host.*meth)(context,this->arg1,this->arg2,this->arg3); }

            OBJECT &host;
            METHOD  meth;
        };

    }

}

#endif


