//! \file

#ifndef Y_Field_Layout_Included
#define Y_Field_Layout_Included 1

#include "y/field/layout/metrics.hpp"
#include "y/field/layout/width.hpp"
#include "y/type/copy.hpp"

#include <iostream>

namespace Yttrium
{

    namespace Field
    {
        //______________________________________________________________________
        //
        //
        //
        //! Generic Layout
        //
        //
        //______________________________________________________________________
        template <typename COORD>
        class Layout : public LayoutWidth<COORD>, public LayoutMetrics
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const unsigned DIMENSION = sizeof(COORD)/sizeof(unit_t); //!< space dimension
            using LayoutWidth<COORD>::width;
          
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Layout() noexcept
            {
                Y_STATIC_ZVAR(Coerce(lower));
                Y_STATIC_ZVAR(Coerce(upper));
            }

            //! copy
            inline Layout(const Layout &other) noexcept :
            LayoutWidth<COORD>(other),
            LayoutMetrics(other),
            lower(other.lower),
            upper(other.upper)
            {
            }

            //! setup
            inline explicit Layout(COORD lo, COORD up) noexcept :
            LayoutWidth<COORD>(lo),
            LayoutMetrics(DIMENSION,C2U(lo),C2U(up), C2U(width) ),
            lower(lo),
            upper(up)
            {
            }

            //! setup with direct copy
            inline explicit Layout(const CopyOf_ &, COORD lo, COORD up, COORD ww, const size_t nn) noexcept :
            LayoutWidth<COORD>(ww),
            LayoutMetrics(DIMENSION,nn),
            lower(lo),
            upper(up)
            {
                assert( validateLayout(C2U(lower),C2U(upper),C2U(width)) );
            }

            //! display metrics
            inline friend std::ostream & operator<<(std::ostream &os, const Layout &l)
            {
                os << "#{[" << l.lower << "->" << l.upper << "]:" << l.width << "}=" << l.items;
                return os;
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const COORD lower; //!< lower coordinate
            const COORD upper; //!< upper coordinate

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

