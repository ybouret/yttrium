
//! \file

#ifndef Y_Ink_Gradient_Filters_Included
#define Y_Ink_Gradient_Filters_Included 1

#include "y/ink/ops/gradient.hpp"

namespace Yttrium
{

    namespace Ink
    {
        
        template <typename T, typename FILTER>
        class GradientFilters : public Gradient<T>
        {
        public:
            typedef typename Gradient<T>::FilterType FilterType;

            inline explicit GradientFilters() : Gradient<T>(FILTER::Name), dxf(), dyf() {}
            inline virtual ~GradientFilters() noexcept {}

            inline virtual const FilterType & dx() const noexcept { return dxf; }
            inline virtual const FilterType & dy() const noexcept { return dyf; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradientFilters);
            const SquareFilter<T,typename FILTER::X> dxf;
            const SquareFilter<T,typename FILTER::Y> dyf;
        };
    }

}

#endif

