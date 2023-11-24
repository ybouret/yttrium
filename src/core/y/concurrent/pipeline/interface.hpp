//! \file

#ifndef Y_Concurrent_Interface_Included
#define Y_Concurrent_Interface_Included 1

#include "y/concurrent/pipeline/task.hpp"
#include "y/container/readable.hpp"

#include "y/object.hpp"
#include "y/counted.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        typedef uint32_t TaskID;



        //______________________________________________________________________
        //
        //
        //
        //! Interface to a Pipeline to process jobs
        //
        //
        //______________________________________________________________________
        class Pipeline : public Readable<const ThreadContext>
        {
        public:
            virtual ~Pipeline() noexcept; //!< cleanup

            TaskID push(const Task &task);



        protected:
            explicit Pipeline() noexcept; //!< setup
            void     upgrade()  noexcept; //!< jobID++, controlled

            const TaskID tid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);

            virtual void   suspend() noexcept          = 0; //!< suspend mechanism
            virtual void   restart() noexcept          = 0; //!< restart mechanism
            virtual TaskID enqueue(const Task &, const TaskID) = 0;
        };

    }

}

#endif

