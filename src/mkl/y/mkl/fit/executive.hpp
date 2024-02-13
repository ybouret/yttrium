//! \file

#ifndef Y_Fit_Executive_Included
#define Y_Fit_Executive_Included 1

#include "y/mkl/fit/step-inventor.hpp"
#include "y/mkl/fit/least-squares/roll.hpp"
#include "y/mkl/interval.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            //! helper to show process
#define Y_MKL_FIT(MSG) do { if(verbose) std::cerr << MSG << std::endl; } while(false)
            

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
                typedef Interval<ABSCISSA>                  IntervalType;     //!< alias
                typedef Readable<IntervalType>              Intervals;        //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline explicit Executive(const bool verb=true) :
                ProxyType(),
                mine( new LeastSquaresType() ),
                roll( new RollType()         ),
                solv( new StepInventorType() ),
                verbose(verb)
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

                template <
                typename FUNCTION,
                typename GRADIENT,
                typename SAMPLE>
                bool run(FUNCTION           &F,
                         SAMPLE             &S,
                         Writable<ABSCISSA> &aorg,
                         const Booleans     &used,
                         GRADIENT           &G,
                         const Intervals    &domain)
                {

                    assert( aorg.size() == used.size() ) ;
                    assert( aorg.size() == domain.size() );

                    // compute full metrics
                    ABSCISSA D2_org = D2(F,S,aorg,used,G);
                    bool     kept   = true;
                    int      p      = -4;

                    Y_MKL_FIT("D2_org = " << D2_org << "@" << aorg << ", p=" << p);

                    if(!solv->buildStep(*mine,p,used,kept))
                    {
                        Y_MKL_FIT("no possible step");
                        return false;
                    }
                    


                    Y_MKL_FIT("step=" << solv->step);

                    return false;
                }




            private:
                Y_DISABLE_COPY_AND_ASSIGN(Executive);
                AutoPtr<LeastSquaresType> mine;
                AutoPtr<RollType>         roll;
                AutoPtr<StepInventorType> solv;

                inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept { return *mine; }
            public:
                bool verbose; //!< verbosity
            };
        }

    }

}

#endif

