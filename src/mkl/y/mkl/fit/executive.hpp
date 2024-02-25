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
                inline explicit Executive(const bool verb=true) :
                ProxyType(),
                dtol(1e-4),
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
                Result run(FUNCTION           &F,
                           SAMPLE             &S,
                           Writable<ABSCISSA> &aorg,
                           const DomainType   &adom,
                           const Booleans     &used,
                           GRADIENT           &G)
                {

                    //__________________________________________________________
                    //
                    // sanity check
                    //__________________________________________________________
                    assert( aorg.size() == used.size() ) ;
                    assert( aorg.size() == adom.size() );
                    assert( adom.contains(aorg) );


                    //__________________________________________________________
                    //
                    // top level initialization
                    //__________________________________________________________
                    Writable<ABSCISSA>       & atry = solv->atry;          // alias
                    const Readable<ABSCISSA> & beta = mine->beta;          // alias
                    //const Readable<ABSCISSA> & step = solv->step;          // alias
                    //Writable<ABSCISSA>       & atmp = solv->atmp;          // alias
                    const int                  pmin = solv->pmin;          // alias
                    const int                  pmax = solv->pmax;          // alias
                    const size_t               nvar = aorg.size();         // num variables
                    int                        p    = -4;                  // initial guess TODO
                    //D2Call<FUNCTION,SAMPLE>    H    = { nvar, aorg, atry, atmp, F,S, *this };

                    //__________________________________________________________
                    //
                    // top level memory
                    //__________________________________________________________
                    solv->prepare(nvar); // workspace

                    Libc::OutputFile fp("D2.dat");
                    ABSCISSA      D2org = D2(F,S,aorg,used,G); // full metrics
                    unsigned long cycle = 0;
                CYCLE:
                    const int p0 = p;
                    ++cycle;
                    Y_MKL_FIT("-------- cycle = " << cycle << " --------");
                    Y_MKL_FIT("D2org = " << D2org << "# @" << aorg << ", p=" << p << ", used=" << used );
                    Y_MKL_FIT("beta  = " << beta);
                    Y_MKL_FIT("curv  = " << mine->curv);

                    fp("%lu %.15g\n", cycle, double(D2org) ).flush();

                    //----------------------------------------------------------
                    //
                    // compute predicted step
                    //
                    //----------------------------------------------------------
                BUILD_STEP:
                    if(!solv->buildStep(*mine,aorg,adom,p,used,verbose))
                    {
                        Y_MKL_FIT("-- no possible step");
                        return Failure;
                    }

                    //----------------------------------------------------------
                    //
                    // here, we have an approximated step and trial
                    //
                    //----------------------------------------------------------
                    const ABSCISSA D2try = D2(F,S,atry);
                    Y_MKL_FIT("D2try = " << D2try << "# @" << atry << ", p=" << p);
#if 0
                    const ABSCISSA sigma = mine->dot(beta,step);
                    const ABSCISSA gamma = mine->xadd(D2try,-D2org,sigma);
                    Y_MKL_FIT("sigma = " << sigma);
                    Y_MKL_FIT("gamma = " << gamma);


                    {
                        const String     fn = Formatted::Get("d2-%lu.dat",cycle);
                        Libc::OutputFile d2(fn);
                        const size_t     nn = 100;
                        for(size_t i=0;i<=nn;++i)
                        {
                            const ABSCISSA u = ABSCISSA(i) / ABSCISSA(nn);
                            d2("%.15g %.15g %.15g\n", double(u), double(H(u)), double(D2org-sigma*u + gamma * u *u));
                        }
                    }
#endif

                    if(D2try>D2org)
                    {
                        Y_MKL_FIT("Bad!");
                        Y_MKL_FIT_DEGRADE(Spurious);
                        goto BUILD_STEP;
                    }

                    Y_MKL_FIT("-- accepted!");


                    bool           success = false;
                    const bool     kept    = (p==p0);
                    if(kept)
                    {
                        Y_MKL_FIT("-- upgrade parameter");
                        if(--p<=pmin) p = pmin;

                        const ABSCISSA delta = D2org-D2try;
                        const ABSCISSA limit = dtol * D2org;
                        Y_MKL_FIT("-- delta = " << delta << " / limit=" << limit);
                        if( delta <= limit)
                        {
                            Y_MKL_FIT("-- success");
                            success = true;
                        }

                    }

                    //----------------------------------------------------------
                    // update and check status
                    //----------------------------------------------------------
                    Tao::Load(aorg,atry);
                    if( success )
                        return Success;


                    //----------------------------------------------------------
                    // prepare for next cycle
                    //----------------------------------------------------------
                    D2org = D2(F,S,aorg,used,G);
                    goto CYCLE;
                }

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


            public:
                bool verbose; //!< verbosity
            };
        }

    }

}

#endif

