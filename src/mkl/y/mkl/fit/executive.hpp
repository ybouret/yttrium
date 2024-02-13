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
                typedef Domain<ABSCISSA>                    DomainType;       //!< alias

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
                         const DomainType   &adom,
                         const Booleans     &used,
                         GRADIENT           &G)
                {

                    assert( aorg.size() == used.size() ) ;
                    assert( aorg.size() == adom.size() );
                    assert( adom.contains(aorg) );

                    //__________________________________________________________
                    //
                    // top level initialization
                    //__________________________________________________________
                    ABSCISSA            Dorg = D2(F,S,aorg,used,G); // full metrics
                    Writable<ABSCISSA> &atry = solv->atry;          // alias
                    const size_t        nvar = aorg.size();         // num variables
                    bool                kept = true;                // indicator
                    int                 p    = -4;                  // initial guess TODO
                    solv->prepare(nvar);                            // workspace


                    Y_MKL_FIT("Dorg = " << Dorg << "# @" << aorg << ", p=" << p);

                    // compute probable step
                    if(!solv->buildStep(*mine,aorg,adom,p,used,kept,verbose))
                    {
                        Y_MKL_FIT("no possible step");
                        return false;
                    }
                    

                    ABSCISSA Dtry = D2(F,S,atry);
                    Y_MKL_FIT("Dtry = " << Dtry << "# @" << atry << ", p=" << p);
                    

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

