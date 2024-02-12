
#include "y/mkl/fit/executive.hpp"
#include "y/mkl/fit/sequential/wrapper.hpp"
#include "y/mkl/fit/sample/heavy.hpp"
#include "y/mkl/fit/sample/light.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/v2d.hpp"
#include "y/container/cxx/array.hpp"
#include "y/mkl/tao/seq/level1.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    class F1D
    {
    public:
        explicit F1D() noexcept {}

        virtual ~F1D() noexcept {}

        inline T F(const T              &t,
                    const Readable<T>    &aorg,
                   const Fit::Variables &vars)
        {
            const T t0 = vars(aorg,"t0");
            const T D  = vars(aorg,"D");
            if(t<=t0) return 0;
            return sqrt( D*(t-t0) );
        }

        inline void G(Writable<double>     &dFda,
                      const T              &t,
                      const Readable<T>    &aorg,
                      const Fit::Variables &vars,
                      const Fit::Booleans  &used)
        {
            const Fit::Variable &t0_ = vars["t0"];
            const Fit::Variable &D__ = vars["D"];

            const T t0 = t0_(aorg);
            const T D  = D__(aorg);
            if(t<=t0) return;
            const T dt  = t-t0;
            const T den = Twice( sqrt( D*dt ) );

            if( t0_(used) ) t0_(dFda) = -D/den;
            if( D__(used) ) D__(dFda) = dt/den;
         }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(F1D);
    };

    template <typename T>
    class Circle
    {
    public:
        typedef V2D<T> VTX;

        explicit Circle()
        {
        }

        virtual ~Circle() noexcept
        {
        }

        inline VTX F(const T              &t,
                     const Readable<T>    &aorg,
                     const Fit::Variables &vars)
        {
            const T radius  = vars(aorg,"radius");
            const T x_c     = vars(aorg,"x_c");
            const T y_c     = vars(aorg,"y_c");
            const double  c = cos(t);
            const double  s = sin(t);
            return VTX(x_c+radius*c,y_c+radius*s);
        }

        inline void G(Writable<VTX>        &dFda,
                      const T              &t,
                      const Readable<T>    &,
                      const Fit::Variables &vars,
                      const Fit::Booleans  &used)
        {
            const Fit::Variable &radius_ = vars["radius"];
            const Fit::Variable &x_c_    = vars["x_c"];
            const Fit::Variable &y_c_    = vars["y_c"];
            //const T radius  = radius_(aorg);
            //const T x_c     = x_c_(aorg);
            //const T y_c     = y_c_(aorg);
            const double  c = cos(t);
            const double  s = sin(t);

            if( radius_(used) ) radius_(dFda) = VTX(c,s);
            if( x_c_(used)    ) x_c_(dFda)    = VTX(1,0);
            if( y_c_(used)    ) y_c_(dFda)    = VTX(0,1);
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Circle);
    };

}

Y_UTEST(fit_samples)
{

    static const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    static const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
    static const size_t _n1   = sizeof(_t1)/sizeof(_t1[0]);
    double _z1[_n1] = { 0 };

    static const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    static const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
    static const size_t _n2   = sizeof(_t2)/sizeof(_t2[0]);
    double              _z2[_n2] = { 0 };

    Fit::Samples<double,double>  samples;
    Fit::Sample<double,double> & S1 = samples( new Fit::LightSample<double,double>("S1",_t1,_x1, _z1, _n1) );
    Fit::Sample<double,double> & S2 = samples( new Fit::LightSample<double,double>("S2",_t2,_x2, _z2, _n2) );

    Fit::HeavySample<double, V2D<double> >    *P1 = new Fit::HeavySample<double, V2D<double> >("H1");
    Fit::Sample<double, V2D<double> >::Pointer H1 = P1;




    P1->add(0.1,  V2D<double>(1,0)  );
    P1->add(1.57, V2D<double>(0,1)  );
    P1->add(3,    V2D<double>(-1,0) );
    P1->add(4.7,  V2D<double>(0,-1) );


    std::cerr << S1 << std::endl;
    std::cerr << S2 << std::endl;
    std::cerr << H1 << std::endl;

    S1.saveDatFile();
    S2.saveDatFile();
    H1->saveDatFile();

    S1.prepare();
    S2.prepare();
    H1->prepare();

    Fit::StepInventor<double> inventor;

    {
        Fit::Variables all;
        all << "t0" << "D1" << "D2";
        std::cerr << "all =" << all << std::endl;

        Fit::Variables &var1 = S1.vars;
        var1.link( all["t0"] );
        var1.link( "D", all["D1"]);
        std::cerr << "var1=" << var1 << std::endl;

        Fit::Variables &var2 = S2.vars;
        var2.link( all["t0"] );
        var2.link( "D", all["D2"]);
        std::cerr << "var2=" << var2 << std::endl;



        Vector<double> aorg(all.span(),0);
        std::cerr << "aorg=" << aorg << std::endl;

        all(aorg,"t0") = -100.0;
        var1(aorg,"D") = 0.1;
        var2(aorg,"D") = 0.12;


        all.display( "(all) ", std::cerr, aorg);
        std::cerr << std::endl;
        var1.display("(v1)  ", std::cerr, aorg);
        std::cerr << std::endl;
        var2.display("(v2)  ", std::cerr, aorg);
        std::cerr << std::endl;


        F1D<double> f1;

        Fit::LeastSquares<double,double>                 eval;
        Fit::LeastSquares<double,double>::OutOfOrderFunc F( &f1, & F1D<double>::F );
        Fit::SequentialWrapper<double, double>           Fw( F );
        Fit::LeastSquares<double,double>::OutOfOrderGrad G( &f1, & F1D<double>::G );

        Y_SIZEOF(eval);

        std::cerr << "--- 1D, sample 1 ---" << std::endl;
        const double D1  = eval.Of(F,S1,aorg);
        const double D1w = eval.Of(Fw,S1,aorg);

        std::cerr << "D1   = " << D1 << " / " << D1w << std::endl;
        Vector<bool>   used(all.span(),true);
        const double   D1a = eval.Of(F,S1, aorg, used, G);
        std::cerr << "D1a  = " << D1a << " / " << D1 << std::endl;
        std::cerr << "beta = " << eval.beta << std::endl;
        std::cerr << "curv = " << eval.curv << std::endl;
        inventor.compute(eval, -2, used);
        std::cerr << std::endl;

        std::cerr << "--- 1D, sample 2 ---" << std::endl;
        const double D2a = eval.Of(F,S2, aorg);
        const double D2b = eval.Of(F,S2, aorg, used, G);
        std::cerr << "D2a  = " << D2a << " / " << D2b << std::endl;
        std::cerr << "beta = " << eval.beta << std::endl;
        std::cerr << "curv = " << eval.curv << std::endl;
        inventor.compute(eval, -2, used);
        std::cerr << std::endl;

        std::cerr << "--- 1D, sample 1+2 ---" << std::endl;
        Fit::LeastSquaresRoll<double,double> roll1D;
        
        const double D2sum  = eval.Of(F,samples,roll1D.setup(samples.size()), aorg);
        const double D2sumw = eval.Of(Fw,samples,roll1D, aorg);
        std::cerr << "D2sum  = " << D2sum << " / " << (D1a*S1.numPoints()+D2a*S2.numPoints())/( S1.numPoints()+ S2.numPoints() ) << " / " << D2sumw << std::endl;

        const double D2full = eval.Of(F,samples,roll1D,aorg,used,G);
        std::cerr << "D2full = " << D2full << std::endl;
        std::cerr << "beta   = " << eval.beta << std::endl;
        std::cerr << "curv   = " << eval.curv << std::endl;

        inventor.compute(eval, -2, used);

        std::cerr << std::endl;
        std::cerr << "---- with partial fit" << std::endl;
        all(used,"t0") = false;
        (void) eval.Of(F,samples,roll1D,aorg,used,G);
        CxxArray<double> step1(used.size(),0);
        CxxArray<double> step2(used.size(),0);

        inventor.compute(*roll1D.head,-2,used);
        Tao::Load(step1,inventor.step);
        inventor.compute(*roll1D.tail,-2,used);
        Tao::Load(step2,inventor.step);
        std::cerr << "step1=" << step1 << std::endl;
        std::cerr << "step2=" << step2 << std::endl;

        inventor.compute(eval,-2,used);
        std::cerr << "stepA=" << inventor.step << std::endl;



        std::cerr << std::endl;

        std::cerr << "-------- Using Executive --------" << std::endl;
        Fit::Executive<double,double> xfit;

        used.ld(true);
        const double S1D2  = xfit.D2(F,S1,aorg);
        const double S1D2w = xfit.D2(Fw,S1,aorg);
        const double S2D2  = xfit.D2(F,S2,aorg);
        const double S2D2w = xfit.D2(Fw,S2,aorg);
        std::cerr << "S1D2 = " << S1D2 << " / " << S1D2w << std::endl;
        std::cerr << "S2D2 = " << S2D2 << " / " << S2D2w << std::endl;

        const double S1D2a = xfit.D2(F,S1,aorg,used,G);
        std::cerr << "S1D2a = " << S1D2a << " / " << S1D2 << std::endl;
        std::cerr << "beta  = " << xfit->beta << std::endl;
        std::cerr << "curv  = " << xfit->curv << std::endl;

        const double S2D2a = xfit.D2(F,S2,aorg,used,G);
        std::cerr << "S2D2a = " << S2D2a << " / " << S2D2 << std::endl;
        std::cerr << "beta  = " << xfit->beta << std::endl;
        std::cerr << "curv  = " << xfit->curv << std::endl;

        {
            const double SAD2  = xfit.D2(F,samples,aorg);
            const double SAD2a = xfit.D2(F,samples,aorg,used,G);
            std::cerr << "SAD2 = " << SAD2 << " / " << SAD2a << std::endl;
            std::cerr << "beta  = " << xfit->beta << std::endl;
            std::cerr << "curv  = " << xfit->curv << std::endl;
        }

        std::cerr << "-- with fixed parameter" << std::endl;
        all(used,"t0") = false;
        {
            const double SAD2  = xfit.D2(F,samples,aorg);
            const double SAD2a = xfit.D2(F,samples,aorg,used,G);
            std::cerr << "SAD2 = " << SAD2 << " / " << SAD2a << std::endl;
            std::cerr << "beta  = " << xfit->beta << std::endl;
            std::cerr << "curv  = " << xfit->curv << std::endl;
        }
    }

#if 1
    {
        std::cerr << "--- 2D ---" << std::endl;
        Fit::Variables &vars = H1->vars;
        vars << "radius" << "x_c" << "y_c";
        typedef V2D<double> VTX;
        Circle<double>      Circ;

        Fit::LeastSquares<double,VTX>                 eval;
        Fit::LeastSquares<double,VTX>::OutOfOrderFunc F( &Circ, & Circle<double>::F );
        Fit::LeastSquares<double,VTX>::OutOfOrderGrad G( &Circ, & Circle<double>::G );
        Y_SIZEOF(eval);


        Vector<double> aorg(vars.span(),0);
        vars(aorg,"x_c")    = 0.0;
        vars(aorg,"y_c")    = 0.0;
        vars(aorg,"radius") = 0.78;
        vars.display("", std::cerr, aorg);

        const double D21 = eval.Of(F, *H1, aorg);
        std::cerr << "D21   = " << D21 << std::endl;

        Vector<bool>   used(aorg.size(),true);
        const double   D21a = eval.Of(F,*H1, aorg, used, G);;
        std::cerr << "D21a  = " << D21a << std::endl;
        std::cerr << "beta  = " << eval.beta << std::endl;
        std::cerr << "curv  = " << eval.curv << std::endl;

        inventor.compute(eval, -2, used);


        vars(used,"radius") = false;
        const double   D21b = eval.Of(F,*H1, aorg, used, G);
        std::cerr << "D21b  = " << D21b << std::endl;
        std::cerr << "beta  = " << eval.beta << std::endl;
        std::cerr << "curv  = " << eval.curv << std::endl;

    }
#endif


    std::cerr << std::endl;
    Y_SIZEOF(Fit::StepInventor<float>);
    Y_SIZEOF(Fit::StepInventor<double>);
    Y_SIZEOF(Fit::StepInventor<long double>);
    Y_SIZEOF(Fit::StepInventor< XReal<float> >);
    Y_SIZEOF(Fit::StepInventor< XReal<double> >);
    Y_SIZEOF(Fit::StepInventor< XReal<long double> >);

    typedef Fit::LeastSquares<float,float>                                    LeastSquaresSmall;
    typedef Fit::LeastSquares< XReal<long double>,V3D< XReal<long double> > > LeastSquaresGreat;

    Y_SIZEOF(LeastSquaresSmall);
    Y_SIZEOF(LeastSquaresGreat);

    typedef Fit::LeastSquaresRoll<float,float>                                    LeastSquaresRollSmall;
    typedef Fit::LeastSquaresRoll< XReal<long double>,V3D< XReal<long double> > > LeastSquaresRollGreat;

    Y_SIZEOF(LeastSquaresRollSmall);
    Y_SIZEOF(LeastSquaresRollGreat);


    typedef Fit::Executive<float,float>                                    ExecutiveSmall;
    typedef Fit::Executive< XReal<long double>,V3D< XReal<long double> > > ExecutiveGreat;
    Y_SIZEOF(ExecutiveSmall);
    Y_SIZEOF(ExecutiveGreat);



}
Y_UDONE()
