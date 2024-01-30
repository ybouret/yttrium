//! \file

#ifndef Y_Field_Layout_Included
#define Y_Field_Layout_Included 1

#include "y/field/layout/metrics.hpp"
#include "y/field/layout/width.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Field
    {
        template <typename COORD>
        class Layout : public LayoutWidth<COORD>, public LayoutMetrics
        {
        public:
            static const unsigned DIMENSION = sizeof(COORD)/sizeof(unit_t);
            using LayoutWidth<COORD>::width;

            inline virtual ~Layout() noexcept
            {
                Y_STATIC_ZVAR(Coerce(lower));
                Y_STATIC_ZVAR(Coerce(upper));
            }

            inline Layout(const Layout &other) noexcept :
            LayoutWidth<COORD>(other),
            LayoutMetrics(other),
            lower(other.lower),
            upper(other.upper)
            {
            }

            inline explicit Layout(COORD lo, COORD up) noexcept :
            LayoutWidth<COORD>(lo),
            LayoutMetrics(DIMENSION,C2U(lo),C2U(up), C2U(width) ),
            lower(lo),
            upper(up)
            {
            }

            inline friend std::ostream & operator<<(std::ostream &os, const Layout &l)
            {
                os << "#[" << l.lower << "->" << l.upper << "]=" << l.width;
                return os;
            }

            const COORD lower;
            const COORD upper;

        private:
            Y_DISABLE_ASSIGN(Layout);
            static inline unit_t * C2U(const COORD &coord) noexcept
            {
                return Memory::OutOfReach::Cast<unit_t,COORD>( &Coerce(coord) );
            }

        };
    }

}

#endif

