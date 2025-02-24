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
            //! quality of a family w.r.t space dimensions
            //__________________________________________________________________
            enum Quality
            {
                Degenerate, // size = 0
                Fragmental, // size < dimensions-1
                Hyperplane, // size == dimensions-1
                Foundation  // size == dimensions
            };

            //__________________________________________________________________
            //
            //
            //
            //! Metrics to keep track of working dimensions
            //
            //
            //__________________________________________________________________
            class Metrics
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Metrics(const size_t) noexcept;         //!< setup, dims>0
                Metrics(const Metrics &)       noexcept;         //!< copy
                virtual ~Metrics() noexcept;                     //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                Quality             getQuality(const size_t)     const noexcept; //!< compute quality
                static const char * QualityText(const Quality quality) noexcept; //!< human readable quality
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const size_t dimensions;                         //!< dimensions>0 of working space
                const size_t concluding;                         //!< dimensions-1 = hyperplane dimensions

            private:
                Y_DISABLE_ASSIGN(Metrics);
            };
        }
    }
}

#endif

