//! \file

#ifndef Y_Concurrent_Loop_Included
#define Y_Concurrent_Loop_Included 1

#include "y/concurrent/loop/kernel.hpp"
#include "y/concurrent/thread/contexts.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //! Wrapper to call any kernel-like function/object
            //
            //__________________________________________________________________
            template <typename KERNEL>
            class Crux : public Kernel
            {
            public:
                inline explicit Crux(KERNEL &k) noexcept : Kernel(), kernel(k) {}         //!< setup
                inline virtual ~Crux()          noexcept {}                               //!< cleanup
                inline virtual void operator()(const ThreadContext &ctx) { kernel(ctx); } //!< forward call
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Crux);
                KERNEL &kernel;
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Interface to Loop a SYNCHRONOUS kernel in single/multiple thread
        //
        //
        //______________________________________________________________________
        class Loop : public ThreadContexts
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void       run1(Kernel &) noexcept = 0; //!< execute the kernel, return when all done
            virtual Lockable & sync()         noexcept = 0; //!< synchronzing object


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            //! wrapper to any kernel(context) interface
            template <typename KERNEL> inline
            void operator()(KERNEL &kernel) {
                Nucleus::Crux<KERNEL> k(kernel);
                run1(k);
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Loop() noexcept; //!< cleanup
        protected:
            explicit Loop() noexcept; //!< setup
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Loop);
        };

        //______________________________________________________________________
        //
        //! alias to share Loop in Engines
        //______________________________________________________________________
        typedef ArcPtr<Loop> SharedLoop;


    }

}

#endif

