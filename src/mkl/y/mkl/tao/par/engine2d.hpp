

//! \file

#ifndef Y_MKL_TAO_Engine2D_Included
#define Y_MKL_TAO_Engine2D_Included 1

#include "y/concurrent/resource/in2d.hpp"

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
            //! Strip matching Tiling<size_t>::Segment
            //
            //
            //__________________________________________________________________
            struct Strip
            {
                size_t icol; //!< initial column
                size_t irow; //!< initial row
                size_t ncol; //!< number of columns
                size_t cend; //!< end column

                //! display
                friend std::ostream &operator<<(std::ostream &os, const Strip &s);
            };

            //__________________________________________________________________
            //
            //
            //
            //! 2D engines for Matrices
            //
            //
            //__________________________________________________________________
            class Engine2D : public Concurrent::Resource2D<size_t>
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
