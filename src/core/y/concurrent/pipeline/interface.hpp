//! \file

#ifndef Y_Concurrent_Interface_Included
#define Y_Concurrent_Interface_Included 1

#include "y/concurrent/thread/context.hpp"
#include "y/container/readable.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        typedef uint32_t                                 JobID;
        typedef Functor<void,TL1(const ThreadContext &)> Job;

        

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


            //! suspend, enqueue, restart
            JobID    push(Job *);

            template <
            typename OBJECT,
            typename METHOD
            > inline JobID push(OBJECT &host, METHOD meth)
            {
                push( new Job(&host,meth) );
            }


        protected:
            explicit Pipeline() noexcept; //!< setup
            void     upgrade()  noexcept; //!< jobID++, controlled

            const JobID jobID;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pipeline);

            virtual void   suspend() noexcept          = 0; //!< suspend mechanism
            virtual void   restart() noexcept          = 0; //!< restart mechanism
            virtual JobID  enqueue(Job *, const JobID) = 0; //!< enqueue a new job, taken care of

        };

    }

}

#endif

