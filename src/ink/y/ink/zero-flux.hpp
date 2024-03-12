
//! \file

#ifndef Y_Ink_Zero_Flux_Included
#define Y_Ink_Zero_Flux_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Ensuring ZeroFlux for operators
        //
        //
        //______________________________________________________________________
        class ZeroFlux
        {
        public:
            typedef unit_t (ZeroFlux::*Get)(const unit_t) const; //!< alias

            ZeroFlux(const unit_t dim) noexcept;
            ZeroFlux(const ZeroFlux &) noexcept;
            ~ZeroFlux()                noexcept;

            unit_t operator[](const unit_t indx) const noexcept;

            const unit_t size; //!< size>=1
            const unit_t symm; //!< 2*(size-1)
        private:
            Get const    proc;

        private:
            Y_DISABLE_ASSIGN(ZeroFlux);

            unit_t Get0(const unit_t) const noexcept;
            unit_t GetN(const unit_t) const noexcept;
        };

    }

}

#endif
