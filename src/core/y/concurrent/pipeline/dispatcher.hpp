
//! \file


#ifndef Y_Concurrent_Pipeline_Dispatcher_Included
#define Y_Concurrent_Pipeline_Dispatcher_Included 1

#include "y/concurrent/pipeline/dispatcher/compute.hpp"
#include "y/concurrent/pipeline/dispatcher/mission.hpp"
#include "y/concurrent/pipeline/dispatcher/connect.hpp"
#include "y/concurrent/resources.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Dispatch calls to methods of ENGINE on multiple ENGINEs
        //
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class Dispatcher : public Resources< Compute<ENGINE> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef Compute<ENGINE> ComputeEngine; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize engines
            explicit Dispatcher(const Concurrent::SharedPipeline &shared) :
            Resources<ComputeEngine>(shared),
            pipeline(shared)
            {
                this->init();
            }


            //! cleanup engines
            virtual ~Dispatcher() noexcept { this->quit(); }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! flush pipeline
            inline void flush() noexcept { pipeline->flush(); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dispatcher);
            friend class DispatcherConnect;
            Concurrent::SharedPipeline pipeline;

        };

        template <typename ENGINE> inline
        TaskUUID DispatcherConnect:: Load(Dispatcher<ENGINE> &kernel, const Task &task)
        {
            return kernel.pipeline->load(task);
        }
    }

}

#endif

