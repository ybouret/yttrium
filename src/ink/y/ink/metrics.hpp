//! \file

#ifndef Y_Ink_Metrics_Included
#define Y_Ink_Metrics_Included 1

#include "y/ink/area.hpp"
#include "y/ink/zero-flux.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Area + stride, bpp and fluxes
        //
        //
        //______________________________________________________________________
        class Metrics : public Area
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup WxHxB
            explicit Metrics(const unit_t    W,
                             const unit_t    H,
                             const unsigned  B) noexcept;
            
            //! setup WxHxB, stride in bytes
            explicit Metrics(const unit_t    W,
                             const unit_t    H,
                             const unsigned  B,
                             const unit_t    S) noexcept;

            //! copy
            Metrics(const Metrics &other)     noexcept;

            //! cleanup
            virtual ~Metrics() noexcept;
            


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t   s;   //!< blocks per row = stride
            const size_t   bpp; //!< bytes per pixel
            const ZeroFlux zfh; //!< zero flux for dimension=h
            const ZeroFlux zfw; //!< zero flux for dimension=w
            
        private:
            Y_DISABLE_ASSIGN(Metrics);
        };

    }

}

#endif
