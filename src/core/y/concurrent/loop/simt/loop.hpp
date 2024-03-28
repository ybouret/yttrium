//! \file


#ifndef Y_Concurrent_SIMT_Loop_Included
#define Y_Concurrent_SIMT_Loop_Included 1

#include "y/concurrent/loop/interface.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        //! keep reference for SIMT
        class SIMT_Loop
        {
        public:
            Loop *       operator->()       noexcept;
            const Loop * operator->() const noexcept;


            virtual ~SIMT_Loop() noexcept;
        protected:
            explicit SIMT_Loop(const SharedLoop &) noexcept;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT_Loop);
            Loop * const loop;
        };



    }

}

#endif
