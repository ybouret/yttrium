
//! \file

#ifndef Y_MKL_TAO_Motor1D_Included
#define Y_MKL_TAO_Motor1D_Included 1

#include "y/concurrent/engine/in1d.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            //__________________________________________________________________
            //
            //
            //
            //! 1D engines for [1..size()] objects
            //
            //
            //__________________________________________________________________
            class Motor1D : public Concurrent::Engine1D<size_t>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Motor1D() noexcept; //!< setup
                virtual ~Motor1D() noexcept; //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Motor1D);
                virtual void activate(const Concurrent::ThreadContext &);
                virtual void shutdown() noexcept;

            };
        }
    }
}

#endif
