//! \file


#ifndef Y_Concurrent_SIMT_Loop_Included
#define Y_Concurrent_SIMT_Loop_Included 1

#include "y/concurrent/loop/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        //______________________________________________________________________
        //
        //
        //
        //! keep reference of Loop for SIMT
        //
        //
        //______________________________________________________________________
        class SIMT_Loop
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Loop *       operator->()       noexcept; //!< access
            const Loop * operator->() const noexcept; //!< access

            //! helper to run kernel(ThreadContext)
            template <typename KERNEL> inline
            void run(KERNEL &kernel) {
                Nucleus::Crux<KERNEL> k(kernel);
                loop->run1(k);
            }
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            virtual ~SIMT_Loop()                   noexcept; //!< cleanup
        protected:
            explicit SIMT_Loop(const SharedLoop &) noexcept; //!< setup



        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT_Loop);
            Loop * const loop;
        };



    }

}

#endif
