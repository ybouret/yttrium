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

            //__________________________________________________________________
            //
            //
            //
            //! Managing fit memory and procedure
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA,typename ORDINATE>
            class Executive : public Proxy< const LeastSquares<ABSCISSA,ORDINATE> >
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef LeastSquaresRoll<ABSCISSA,ORDINATE> RollType;         //!< alias
                typedef LeastSquares<ABSCISSA,ORDINATE>     LeastSquaresType; //!< alias
                typedef StepInventor<ABSCISSA>              StepInventorType; //!< alias
                typedef Sample<ABSCISSA,ORDINATE>           SampleType;       //!< alias
                typedef Samples<ABSCISSA,ORDINATE>          SamplesType;      //!< alias
                typedef Proxy<const LeastSquaresType>       ProxyType;        //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline explicit Executive() :
                ProxyType(),
                mine( new LeastSquaresType() ),
                roll( new RollType()         ),
                solv( new StepInventorType() )
                {}

                //! cleanup
                inline virtual ~Executive() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

#include "executive/d2.hpp"

               




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

