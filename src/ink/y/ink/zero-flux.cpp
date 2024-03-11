
#include "y/ink/zero-flux.hpp"

namespace Yttrium
{
    namespace Ink
    {
        ZeroFlux:: ~ZeroFlux() noexcept
        {
            Coerce(size) = 0;
            Coerce(symm) = 0;
        }

        ZeroFlux:: ZeroFlux(const unit_t dim) noexcept :
        size(dim),
        symm(2*(size-1)),
        proc(dim<=1 ? & ZeroFlux::Get0 : & ZeroFlux::GetN )
        {
            
        }

        ZeroFlux:: ZeroFlux(const ZeroFlux &zf) noexcept :
        size( zf.size ),
        symm( zf.symm ),
        proc( zf.proc )
        {
        }

        unit_t ZeroFlux:: Get0(const unit_t) const noexcept
        {
            return 0;
        }

        unit_t ZeroFlux:: GetN(const unit_t indx) const noexcept
        {
            const ZeroFlux &self = *this;
            if(indx<0) {
                return self(-indx);
            }
            else
            {
                if(indx>=size)
                {
                    return self(symm-indx);
                }
                else
                {
                    assert(indx>=0); assert(indx<size);
                    return indx;
                }
            }
        }

        unit_t ZeroFlux:: operator()(const unit_t indx) const noexcept
        {
            assert(0!=proc);
            return ( *this.*proc )(indx);
        }

    }

}
