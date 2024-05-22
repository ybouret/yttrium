//! \file

#ifndef Y_Concurrent_Pipeline_Multiplex_Included
#define Y_Concurrent_Pipeline_Multiplex_Included 1

#include "y/concurrent/pipeline/multiplex/mill.hpp"
#include "y/concurrent/frames.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Mill + ENGINEs
        /**
         - ENGINE Should derive from Frame0D
         - create as many engines as pipeline size
         */
        //
        //______________________________________________________________________
        template <typename ENGINE>
        class Multiplex : public MultiplexMill, public Frames<ENGINE>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Writable<ENGINE> Engines; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup ENGINEs + keep pipeline reference inside MultiplexPipeline
            inline explicit Multiplex(const SharedPipeline &sp) :
            MultiplexMill(sp), Frames<ENGINE>(mill)
            {
            }

            //! cleanup
            inline virtual ~Multiplex() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Multiplex);
        };
    }

}


#endif

