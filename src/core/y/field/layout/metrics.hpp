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

      

      
    }
}

#endif
