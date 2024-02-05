//! \file

#ifndef Y_Field_Layout_Included
#define Y_Field_Layout_Included 1

#include "y/field/layout/metrics.hpp"
#include "y/field/layout/scope.hpp"
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
        template <typename COORD,typename COUNT>
        class Layout : public LayoutScope<COUNT>, public LayoutMetrics
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const unsigned DIMENSION = sizeof(COORD)/sizeof(unit_t); //!< space dimension
            using LayoutScope<COUNT>::width;
            using LayoutScope<COUNT>::shift;
            typedef COORD CoordType;

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
            LayoutScope<COUNT>(other),
            LayoutMetrics(other),
            lower(other.lower),
            upper(other.upper)
            {
            }

            //! setup
            inline explicit Layout(COORD lo, COORD up) noexcept :
            LayoutScope<COUNT>(),
            LayoutMetrics(DIMENSION, C2U(lo), C2U(up), C2S(width), C2S(shift)),
            lower(lo),
            upper(up)
            {
            }
            

            template <typename SUPER> inline
            explicit Layout(const SubLayout_ &_, const SUPER &super) noexcept :
            LayoutScope<COUNT>(_,super),
            LayoutMetrics(DIMENSION, this->lastShift() ),
            lower( Memory::OutOfReach::Conv<const COORD,const typename SUPER::CoordType>(super.lower) ),
            upper( Memory::OutOfReach::Conv<const COORD,const typename SUPER::CoordType>(super.upper) )
            {

            }

            inline friend bool operator==(const Layout &lhs,
                                          const Layout &rhs) noexcept
            {
                if(lhs.lower==rhs.lower && lhs.upper==rhs.upper)
                {
                    assert(lhs.width==rhs.width);
                    assert(lhs.shift==rhs.shift);
                    assert(lhs.items==rhs.items);
                    return true;
                }
                else
                    return false;
            }

            //! display metrics
            inline friend std::ostream & operator<<(std::ostream &os, const Layout &l)
            {
                os << "#{[" << l.lower << "->" << l.upper << "]:" << l.width << "}=" << l.items << " (shift=" << l.shift <<")";
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

            static inline size_t * C2S(const COUNT &count) noexcept
            {
                return Memory::OutOfReach::Cast<size_t,COUNT>( &Coerce(count) );
            }

        };
    }

}

#endif

