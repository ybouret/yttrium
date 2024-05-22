

//! \file

#ifndef Y_Concurrent_Pipeline_Multiplex_Mill_Included
#define Y_Concurrent_Pipeline_Multiplex_Mill_Included 1

#include "y/concurrent/pipeline/interface.hpp"
#include "y/concurrent/defs.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Manager for Multiplex
        //
        //
        //______________________________________________________________________
        class MultiplexMill
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Pipeline *        operator->()       noexcept; //!< access
            const Pipeline  * operator->() const noexcept; //!< access

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~MultiplexMill()                       noexcept; //!< cleanup
        protected:
            explicit MultiplexMill(const SharedPipeline &) noexcept; //!< manual setup
            explicit MultiplexMill(const Sequential_    &);          //!< use Alone()
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SharedPipeline mill;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MultiplexMill);
        };

    }

}

#endif

