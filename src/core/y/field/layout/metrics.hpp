//! \file

#ifndef Y_Field_Layout_Metrics_Included
#define Y_Field_Layout_Metrics_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Field
    {
        //______________________________________________________________________
        //
        //
        //
        //! Shared components for Layouts
        //
        //
        //______________________________________________________________________
        class LayoutMetrics
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~LayoutMetrics()                      noexcept; //!< cleanup
            explicit LayoutMetrics(const LayoutMetrics &) noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned dimension; //!< space dimension
            const size_t   items;     //!< items in field

            //! helper to debug
            bool validateLayout(const unit_t * const lower,
                                const unit_t * const upper,
                                const unit_t * const width) const noexcept;

        protected:
            //! setup with items computation and coordinates ordering
            explicit LayoutMetrics(const unsigned d,
                                   unit_t * const lower,
                                   unit_t * const upper,
                                   unit_t * const width) noexcept;

            //! setup directly from precomputed coords
            explicit LayoutMetrics(const unsigned d, const size_t n) noexcept;

        private:
            Y_DISABLE_ASSIGN(LayoutMetrics);
        };

      

      
    }
}

#endif
