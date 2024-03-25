//! \file

#ifndef Y_CONCURRENT_FRAME_PUNCTUAL_INCLUDED
#define Y_CONCURRENT_FRAME_PUNCTUAL_INCLUDED 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //
            //! Dummy class for Punctual Frame (used with Pipeline)
            //
            //
            //__________________________________________________________________
            class  Punctual
            {
            public:

                explicit Punctual() noexcept; //!< setup
                virtual ~Punctual() noexcept; //!< cleanup
                Y_OSTREAM_PROTO(Punctual);    //!< display

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Punctual);
            };
        }
    }
}

#endif

