//! \file

#ifndef Y_Ink_Metrics_Included
#define Y_Ink_Metrics_Included 1

#include "y/ink/area.hpp"
#include "y/ink/zero-flux.hpp"

namespace Yttrium
{
    namespace Ink
    {

        class Metrics : public Area
        {
        public:
            explicit Metrics(const unit_t    W,
                             const unit_t    H,
                             const unsigned  B) noexcept;
            
            explicit Metrics(const unit_t    W,
                             const unit_t    H,
                             const unsigned  B,
                             const unit_t    S) noexcept;

            Metrics(const Metrics &other)     noexcept;
            Metrics(const Metrics &other, const unsigned bs) noexcept;
            virtual ~Metrics() noexcept;
            
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
