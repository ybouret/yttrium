
//! \file


#ifndef Y_Concurrent_SIMT_Mill_Included
#define Y_Concurrent_SIMT_Mill_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/defs.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Manager for SIMT
        //
        //
        //______________________________________________________________________
        class SIMT_Mill
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
                mill->run1(k);
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~SIMT_Mill() noexcept; //!< cleanup
        protected:
            explicit SIMT_Mill(const SharedLoop &sl) noexcept; //!< manual setup
            explicit SIMT_Mill(const Sequential_  &);          //!< with inner Mono()
            explicit SIMT_Mill(const InParallel_  &);          //!< with inner Crew()

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SharedLoop mill; //!< implementation

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT_Mill);
        };
    }

}

#endif
