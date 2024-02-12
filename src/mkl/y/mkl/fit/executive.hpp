//! \file

#ifndef Y_Fit_Executive_Included
#define Y_Fit_Executive_Included 1

#include "y/mkl/fit/step-inventor.hpp"
#include "y/mkl/fit/least-squares/roll.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            template <typename ABSCISSA,typename ORDINATE>
            class Executive
            {
            public:
                typedef LeastSquaresRoll<ABSCISSA,ORDINATE> RollType;
                typedef LeastSquares<ABSCISSA,ORDINATE>     LeastSquaresType;
                typedef StepInventor<ABSCISSA>              StepInventorType;


                inline explicit Executive() : roll() {}

                inline virtual ~Executive() noexcept {}

                RollType roll;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Executive);
            };
        }

    }

}

#endif

