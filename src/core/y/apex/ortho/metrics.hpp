
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
            //__________________________________________________________________
            //
            //
            //! Metrics to keep track of working dimensions
            //
            //__________________________________________________________________
            class Metrics
            {
            public:
                const size_t dimensions;                 //!< dimensions of working space
                virtual ~Metrics() noexcept;             //!< cleanup

            protected:
                explicit Metrics(const size_t) noexcept; //!< setup, dims>0
                Metrics(const Metrics &)       noexcept; //!< copy

            private:
                Y_DISABLE_ASSIGN(Metrics);
            };
        }

    }

}

#endif
