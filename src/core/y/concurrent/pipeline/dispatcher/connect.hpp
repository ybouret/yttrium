
//! \file


#ifndef Y_Concurrent_Pipeline_Dispatcher_Connect_Included
#define Y_Concurrent_Pipeline_Dispatcher_Connect_Included 1

#include "y/concurrent/pipeline/task/uuid.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        
        template <typename ENGINE> class Dispatcher; //!< forward declaration
        class Task;
        
        //______________________________________________________________________
        //
        //
        //
        //! helper to access dispatcher's data by friendship
        //
        //
        //______________________________________________________________________
        class DispatcherConnect
        {
        public:
            virtual ~DispatcherConnect() noexcept; //!< cleanup

        protected:
            explicit DispatcherConnect() noexcept; //!< setup

            //! access  dispatcher's pipeline
            template <typename ENGINE> static
            TaskUUID Load(Dispatcher<ENGINE> &, const Task &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DispatcherConnect);
        };

    }

}

#endif
