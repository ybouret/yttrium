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
                typedef Sample<ABSCISSA,ORDINATE>           SampleType;     //!< alias
                typedef Samples<ABSCISSA,ORDINATE>          SamplesType;    //!< alias

                inline explicit Executive() :
                roll(   ),
                mine( new LeastSquaresType() ),
                solv( new StepInventorType() )
                {}

                inline virtual ~Executive() noexcept {}

                template <typename FUNCTION> inline
                ABSCISSA LeastSquaresOf(FUNCTION                 &F,
                                        SampleType               &S,
                                        const Readable<ABSCISSA> &aorg)
                {
                    LeastSquaresType &LS = *roll.setup(1).head;
                    return LS.Of(F,S,aorg);
                }

                template <
                typename FUNCTION,
                typename GRADIENT> inline
                ABSCISSA LeastSquaresOf(FUNCTION                 &F,
                                        SamplesType              &S,
                                        const Readable<ABSCISSA> &aorg )
                {
                    return mine->Of(F,S,roll.setup(S.size()),aorg);
                }



                RollType                  roll;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Executive);
                AutoPtr<LeastSquaresType> mine;
                AutoPtr<StepInventorType> solv;


            };
        }

    }

}

#endif

