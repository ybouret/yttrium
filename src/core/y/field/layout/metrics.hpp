//! \file

#ifndef Y_Field_Layout_Metrics_Included
#define Y_Field_Layout_Metrics_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Field
    {
        class LayoutMetrics
        {
        public:

            const unsigned dimension;
            const size_t   items;

            virtual ~LayoutMetrics() noexcept;
            explicit LayoutMetrics(const LayoutMetrics &) noexcept;

        protected:
            explicit LayoutMetrics(const unsigned d,
                                   unit_t * const lower,
                                   unit_t * const upper,
                                   unit_t * const width) noexcept;

        private:
            Y_DISABLE_ASSIGN(LayoutMetrics);
        };

        template <typename COORD> class LayoutWidth
        {
        public:
            inline virtual ~LayoutWidth() noexcept { Y_STATIC_ZVAR( Coerce(width) ); }
            inline explicit LayoutWidth(const COORD       &dummy) noexcept : width(dummy) {}
            inline explicit LayoutWidth(const LayoutWidth &other) noexcept : width(other.width) {}
            
            const COORD width;

        private:
            Y_DISABLE_ASSIGN(LayoutWidth);
        };

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


            const COORD lower;
            const COORD upper;

        private:
            Y_DISABLE_ASSIGN(Layout);
            static inline unit_t * C2U(const COORD &coord) noexcept
            {
                return Memory::OutOfReach::Cast<unit_t,const COORD>( &coord );
            }

        };
    }
}

#endif
