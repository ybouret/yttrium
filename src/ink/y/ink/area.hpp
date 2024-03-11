
//! \file

#ifndef Y_Ink_Area_Included
#define Y_Ink_Area_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Ink
    {

        class Area
        {
        public:
            Area(const unit_t W, const unit_t H) noexcept;
            virtual ~Area()                      noexcept;
            Area(const Area &)                   noexcept;

            const unit_t x;
            const unit_t y;
            const unit_t w;
            const unit_t h;
            const size_t n;
            
        private:
            Y_DISABLE_ASSIGN(Area);

        };

    }
}

#endif
