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
            class Executive : public Proxy< const LeastSquares<ABSCISSA,ORDINATE> >
            {
            public:
                typedef LeastSquaresRoll<ABSCISSA,ORDINATE> RollType;
                typedef LeastSquares<ABSCISSA,ORDINATE>     LeastSquaresType;
                typedef StepInventor<ABSCISSA>              StepInventorType;
                typedef Sample<ABSCISSA,ORDINATE>           SampleType;     //!< alias
                typedef Samples<ABSCISSA,ORDINATE>          SamplesType;    //!< alias
                typedef Proxy<const LeastSquaresType>       ProxyType;

                inline explicit Executive() :
                ProxyType(),
                mine( new LeastSquaresType() ),
                roll( new RollType()         ),
                solv( new StepInventorType() )
                {}

                inline virtual ~Executive() noexcept {}

                template <typename FUNCTION> inline
                ABSCISSA D2(FUNCTION                 &F,
                            SampleType               &S,
                            const Readable<ABSCISSA> &aorg)
                {
                    return mine->Of(F,S,aorg);
                }

                template <typename FUNCTION> inline
                ABSCISSA D2(FUNCTION                 &F,
                            SamplesType              &S,
                            const Readable<ABSCISSA> &aorg)
                {
                    return mine->Of(F,S,roll->setup(S.size()),aorg);
                }


                template <typename FUNCTION, typename GRADIENT> inline
                ABSCISSA D2(FUNCTION                 &F,
                            SampleType               &S,
                            const Readable<ABSCISSA> &aorg,
                            const Booleans           &used,
                            GRADIENT                 &G)
                {
                    return mine->Of(F,S,aorg,used,G);
                }

                template <typename FUNCTION, typename GRADIENT> inline
                ABSCISSA D2(FUNCTION                 &F,
                            SamplesType              &S,
                            const Readable<ABSCISSA> &aorg,
                            const Booleans           &used,
                            GRADIENT                 &G)
                {
                    return mine->Of(F,S,roll->setup(S.size()),aorg,used,G);
                }





            private:
                Y_DISABLE_COPY_AND_ASSIGN(Executive);
                AutoPtr<LeastSquaresType> mine;
                AutoPtr<RollType>         roll;
                AutoPtr<StepInventorType> solv;

                inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept { return *mine; }

            };
        }

    }

}

#endif

