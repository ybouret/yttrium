
//! \file

#ifndef Y_Ink_Filter_Square_Included
#define Y_Ink_Filter_Square_Included 1

#include "y/ink/ops/filter.hpp"

namespace Yttrium
{
    namespace Ink
    {

        namespace Crux
        {
            class SquareFilter
            {
            public:
                explicit SquareFilter(const char * const theName,
                                      const unsigned     numData);
                virtual ~SquareFilter() noexcept;

                const unit_t width_;
                const unit_t delta_;
                const Area   layout() const noexcept
                {
                    return Area( Coord(delta_,delta_), width_, width_ );
                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SquareFilter);
            };
        }


        template <typename T, typename FILTER>
        class SquareFilter : public Crux::SquareFilter, public Filter<T>
        {
        public:
            static const unsigned NumData = sizeof(FILTER::Data)/sizeof(FILTER::Data[0][0]);

            inline explicit SquareFilter() :
            Crux::SquareFilter(FILTER::Name,NumData),
            Filter<T>(FILTER::Name,&FILTER::Data[0][0],layout())
            {
            }

            inline virtual ~SquareFilter() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SquareFilter);
            
        };

    }

}

#endif

