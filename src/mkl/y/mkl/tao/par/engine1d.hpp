
//! \file

#ifndef Y_MKL_TAO_Engine1D_Included
#define Y_MKL_TAO_Engine1D_Included 1

#include "y/concurrent/resource/in1d.hpp"

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
            class Engine1D : public Concurrent::Resource1D<size_t>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Engine1D(const Concurrent::ThreadContext &) noexcept; //!< setup
                virtual ~Engine1D() noexcept;                                  //!< cleanup

                void    *anonymous;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine1D);
                virtual void activate();
                virtual void shutdown() noexcept;

            };
        }
    }
}

#endif
