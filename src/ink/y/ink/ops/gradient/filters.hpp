
//! \file

#ifndef Y_Ink_Gradient_Filters_Included
#define Y_Ink_Gradient_Filters_Included 1

#include "y/ink/ops/gradient.hpp"

namespace Yttrium
{

    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Gradient with built-in filters
        //
        //
        //______________________________________________________________________
        template <typename T, typename FILTER>
        class GradientFilters : public Gradient<T>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename Gradient<T>::FilterType FilterType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit GradientFilters() : Gradient<T>(FILTER::Name), dxf(), dyf() {} //!< setup
            inline virtual ~GradientFilters() noexcept {}                                  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
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

