
//! \file


#ifndef Y_Concurrent_SIMT_Mill_Included
#define Y_Concurrent_SIMT_Mill_Included 1

#include "y/concurrent/loop/interface.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        
        class SIMT_Mill
        {
        public:
            virtual ~SIMT_Mill() noexcept;

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
                mill->run1(k);
            }

        protected:
            explicit SIMT_Mill(const SharedLoop &sl) noexcept;

            SharedLoop mill;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT_Mill);
        };
    }

}

#endif
