
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef unit_t (ZeroFlux::*Get)(const unit_t) const; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            ZeroFlux(const unit_t dim) noexcept; //!< setup dim>0
            ZeroFlux(const ZeroFlux &) noexcept; //!< copy
            ~ZeroFlux()                noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get virtual index to produce null flux operator
            unit_t operator[](const unit_t indx) const noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unit_t size; //!< size>=1
            const unit_t symm; //!< 2*(size-1)
        private:
            Get const    proc;
            Y_DISABLE_ASSIGN(ZeroFlux);
            unit_t Get0(const unit_t) const noexcept;
            unit_t GetN(const unit_t) const noexcept;
        };

    }

}

#endif
