

//! \file

#ifndef Y_Concurrent_Callback_Included
#define Y_Concurrent_Callback_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/type/traits.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //! function or functionoid, using copy/copy constructor
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class Callback : public Runnable
        {
        public:
            inline explicit Callback(const FUNCTION &fn) : Runnable(), function(fn) {} //!< setup
            inline virtual ~Callback() noexcept {}                                     //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback);
            virtual void run(const ThreadContext &context) { function(context); }

            FUNCTION function;
        };


        //______________________________________________________________________
        //
        //
        //! Callback with 1 arg
        //
        //______________________________________________________________________
        template <typename FUNCTION, typename ARG1>
        class Callback1 : public Runnable
        {
        public:

            //! setup
            inline explicit Callback1(const FUNCTION                      &fn,
                                      typename TypeTraits<ARG1>::ParamType a1) :
            Runnable(),
            func(fn),
            arg1(a1)
            {}

            inline virtual ~Callback1() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback1);
            virtual void run(const ThreadContext &context) { func(context,arg1); }

            FUNCTION func;
            ARG1     arg1;
        };
        
    }

}

#endif

