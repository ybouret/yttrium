//! \file

#ifndef Y_Fit_Executive_Included
#define Y_Fit_Executive_Included 1

#include "y/mkl/fit/step-inventor.hpp"
#include "y/mkl/fit/least-squares/roll.hpp"
#include "y/mkl/numeric.hpp"

namespace Yttrium
{

    namespace MKL
    {


        namespace Fit
        {

            enum Result
            {
                Success,
                Failure,
                Spurious
            };

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
                typedef Domain<ABSCISSA>                    DomainType;       //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline explicit Executive() :
                ProxyType(),
                dtol(1e-4),
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
#include "executive/run.hpp"
#include "executive/err.hpp"




                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                ABSCISSA                  dtol;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Executive);
                AutoPtr<LeastSquaresType> mine;
                AutoPtr<RollType>         roll;
                AutoPtr<StepInventorType> solv;

                inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept { return *mine; }

                template <typename FUNCTION, typename SAMPLE>
                struct D2Call
                {
                    size_t                    nvar;
                    const Readable<ABSCISSA> &aorg;
                    const Readable<ABSCISSA> &atry;
                    Writable<ABSCISSA>       &atmp;
                    FUNCTION                 &F;
                    SAMPLE                   &S;
                    Executive                &self;

                    inline ABSCISSA operator()(const ABSCISSA u)
                    {
                        static const ABSCISSA _1(1);
                        const ABSCISSA v = _1 - u;
                        for(size_t i=nvar;i>0;--i)
                        {
                            atmp[i] = v * aorg[i] + u * atry[i];
                        }
                        return self.D2(F,S,atmp);
                    }

                };
                
            };
        }

    }

}

#endif

