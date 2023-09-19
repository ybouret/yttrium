
//! \file

#ifndef Y_Apex_Ortho_Metrics_Included
#define Y_Apex_Ortho_Metrics_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {
            class Metrics
            {
            public:
                virtual ~Metrics() noexcept;

                const size_t dimensions;

            protected:
                explicit Metrics(const size_t) noexcept;
                Metrics(const Metrics &)       noexcept;

            private:
                Y_DISABLE_ASSIGN(Metrics);
            };
        }

    }

}

#endif
