//! \file


#ifndef Y_Concurrent_Pipeline_Dispatcher_Compute_Included
#define Y_Concurrent_Pipeline_Dispatcher_Compute_Included 1

#include "y/concurrent/resource/in0d.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Compute Engine, based on Resource0D and ENGINE
        /**
         helper to parallelize calls to mutliple ENGINEs
         */
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class Compute : public Resource0D, public ENGINE
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup, copy context
            inline explicit Compute(const ThreadContext &ctx) :
            Resource0D(ctx), ENGINE()
            {
            }

            //! cleanup
            inline virtual ~Compute() noexcept
            {

            }

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! default global activate
            virtual void activate() noexcept { }

            //! default global shutdown
            virtual void shutdown() noexcept { }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compute);
        };

    }

}

#endif

