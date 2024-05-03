
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
            //__________________________________________________________________
            //
            //
            //
            //! base class for SquarFilter
            //
            //
            //__________________________________________________________________
            class SquareFilter
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! check that numData is a square
                explicit SquareFilter(const char * const theName,
                                      const unsigned     numData);

                //! cleanup
                virtual ~SquareFilter() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! return Filter area
                const Area layout() const noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const unit_t width_; //!< width of area
                const unit_t delta_; //!< offset of area

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SquareFilter);
            };
        }

        //__________________________________________________________________
        //
        //
        //
        //! SquareFilter from FILTER::Name and FILTER::Data
        //
        //
        //__________________________________________________________________
        template <typename T, typename FILTER>
        class SquareFilter : public Crux::SquareFilter, public Filter<T>
        {
        public:
            //! compute number of coefficients
            static const unsigned NumData = sizeof(FILTER::Data)/sizeof(FILTER::Data[0][0]);

            //! setup
            inline explicit SquareFilter() :
            Crux::SquareFilter(FILTER::Name,NumData),
            Filter<T>(FILTER::Name,&FILTER::Data[0][0],layout())
            {
            }

            //! cleanup
            inline virtual ~SquareFilter() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SquareFilter);
            
        };

    }

}

#endif

