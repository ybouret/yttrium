
//! \file

#ifndef Y_Orthogonal_Metrics_Included
#define Y_Orthogonal_Metrics_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    
    namespace Orthogonal
    {
        //__________________________________________________________________
        //
        //! quality of a family w.r.t space dimensions
        //__________________________________________________________________
        enum Quality
        {
            Fragmental, // size < dimensions-1
            Hyperplane, // size == dimensions-1
            Generating  // size == dimensions
        };

        //__________________________________________________________________
        //
        //
        //! Metrics to keep track of working dimensions
        //
        //__________________________________________________________________
        class Metrics
        {
        public:
            const size_t dimensions;                         //!< dimensions>0 of working space
            const size_t concluding;                         //!< dimensions-1 = hyperplane dimensions
            virtual ~Metrics() noexcept;                     //!< cleanup
            Quality getQuality(const size_t) const noexcept; //!< compute quality

        protected:
            explicit Metrics(const size_t) noexcept;         //!< setup, dims>0
            Metrics(const Metrics &)       noexcept;         //!< copy

        private:
            Y_DISABLE_ASSIGN(Metrics);
        };
    }



}

#endif
