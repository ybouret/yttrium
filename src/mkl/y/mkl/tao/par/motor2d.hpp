

//! \file

#ifndef Y_MKL_TAO_Motor2D_Included
#define Y_MKL_TAO_Motor2D_Included 1

#include "y/concurrent/engine/in2d.hpp"

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
            class Motor2D : public Concurrent::Engine2D<size_t>
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Motor2D() noexcept; //!< setup
                virtual ~Motor2D() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! return Strip[1..size] after activation
                const Strip & operator()(const size_t indx) const noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Motor2D);
                const Strip * const strip;
                virtual void activate(const Concurrent::ThreadContext &); //!< link strip
                virtual void shutdown() noexcept; //!< unlink strip
            };
        }
    }
}

#endif
