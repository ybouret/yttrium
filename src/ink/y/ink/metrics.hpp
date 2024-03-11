

//! \file

#ifndef Y_Ink_Metrics_Included
#define Y_Ink_Metrics_Included 1

#include "y/ink/area.hpp"

namespace Yttrium
{
    namespace Ink
    {

        class Metrics : public Area
        {
        public:
            explicit Metrics(const unit_t    W,
                             const unit_t    H,
                             const unsigned  bs) noexcept;
            Metrics(const Metrics &other)     noexcept;
            Metrics(const Metrics &other, const unsigned bs) noexcept;
            virtual ~Metrics() noexcept;

            const size_t s;   //!< blocks per row
            const size_t b;   //!< bytes per pixel

        private:
            Y_DISABLE_ASSIGN(Metrics);
        };

    }

}

#endif
