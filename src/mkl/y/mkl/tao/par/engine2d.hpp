

//! \file

#ifndef Y_MKL_TAO_Engine2D_Included
#define Y_MKL_TAO_Engine2D_Included 1

#include "y/concurrent/resource/in2d.hpp"
#include "y/mkl/tao/par/xaddptr.hpp"
#include "y/mkl/tao/par/strip.hpp"

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
            //! 2D engines for Matrices
            //
            //
            //__________________________________________________________________
            class Engine2D : public Concurrent::Resource2D<size_t>, public XAddPtr
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Engine2D(const Concurrent::ThreadContext &) noexcept; //!< setup
                virtual ~Engine2D() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! return Strip[1..size] after activation
                const Strip & operator()(const size_t indx) const noexcept;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine2D);
                const Strip * const strip;
                virtual void activate();          //!< link strip
                virtual void shutdown() noexcept; //!< unlink strip
            };
        }
    }
}

#endif
