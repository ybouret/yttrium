

#include "y/mkl/fit/sequential/wrapper.hpp"
#include "y/mkl/fit/sample/heavy.hpp"
#include "y/mkl/fit/sample/light.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/v2d.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/functor.hpp"

using namespace Yttrium;
using namespace MKL;

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {


          
            template <typename ABSCISSA, typename ORDINATE>
            class ComputeD2
            {
            public:
                typedef Sample<ABSCISSA,ORDINATE>      SampleType;
                typedef TypeFor<ABSCISSA,ORDINATE>     MyType;

                typedef typename SampleType::Abscissae       Abscissae;
                typedef typename SampleType::Ordinates       Ordinates;;
                typedef typename MyType::OutOfOrderFunc      OutOfOrderFunc;
                typedef Sequential<ABSCISSA,ORDINATE>        SequentialFunc;
                typedef typename MyType::OutOfOrderGradient  OutOfOrderGrad;

                static const size_t                   Dimension = SampleType::Dimension;


                explicit ComputeD2() : 
                xadd(),
                dFda(),
                zero(0),
                half(0.5),
                z___(),
                zord(*static_cast<const ORDINATE *>(Memory::OutOfReach::Addr(z___)) )
                {

                }

                virtual ~ComputeD2() noexcept
                {

                }

                inline void push(const ORDINATE &Bj, const ORDINATE &Fj)
                {
                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *a  = SampleType::O2A(dB);
                    for(size_t d=0;d<Dimension;++d)
                        xadd << Squared(a[d]);
                }


                inline ABSCISSA Of(SequentialFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars)
                {
                    const size_t     n = S.numPoints();
                    const Abscissae &a = S.abscissae();
                    const Ordinates &b = S.ordinates();

                    xadd.make(n*Dimension);

                    // first point
                    {
                        const size_t   j  = S.indx[1];
                        const ORDINATE Fj = F.set(a[j],aorg,vars);
                        push(b[j],Fj);
                    }

                    // following points
                    for(size_t i=2;i<=n;++i)
                    {
                        const size_t   j  = S.indx[i];
                        const ORDINATE Fj = F.run(a[j],aorg,vars);
                        push(b[j],Fj);
                    }


                    return half * xadd.sum();
                }

                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars)
                {
                    const size_t     n = S.numPoints();
                    const Abscissae &a = S.abscissae();
                    const Ordinates &b = S.ordinates();

                    xadd.make(n*Dimension);

                    for(size_t j=n;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        push(b[j],Fj);
                    }
                    return half * xadd.sum();
                }

                inline void push(Writable<ABSCISSA> &beta,
                                 const ORDINATE     &Bj,
                                 const ORDINATE     &Fj)
                {
                    assert(beta.size()==dFda.size());
                    const ORDINATE  dB = Bj - Fj;
                    const ABSCISSA *df = SampleType::O2A(dB);
                    for(unsigned d=0;d<Dimension;++d)
                        xadd << Squared(df[d]);


                    for(size_t i=1;i<=beta.size();++i)
                    {
                        const ABSCISSA *g = SampleType::O2A(dFda[i]);
                        for(unsigned d=0;d<Dimension;++d)
                        {
                            beta[i] += df[d] * g[d];
                        }
                    }
                }


                inline ABSCISSA Of(OutOfOrderFunc           &F,
                                   const SampleType         &S,
                                   const Readable<ABSCISSA> &aorg,
                                   const Variables          &vars,
                                   const Booleans           &used,
                                   OutOfOrderGrad           &G,
                                   Writable<ABSCISSA>       &beta)

                {
                    const size_t     n = S.numPoints();
                    const Abscissae &a = S.abscissae();
                    const Ordinates &b = S.ordinates();

                    xadd.make(n*Dimension);
                    dFda.adjust(beta.size(),zord);
                    beta.ld(zero);

                    for(size_t j=n;j>0;--j)
                    {
                        const ORDINATE Fj = F(a[j],aorg,vars);
                        dFda.ld(zord);
                        G(dFda,a[j],aorg,vars,used);
                        push(beta,b[j],Fj);
                    }

                    return half * xadd.sum();
                }

                Antelope::Add<ABSCISSA>       xadd;
                Vector<ORDINATE,SampleMemory> dFda;
                const ABSCISSA                zero;
                const ABSCISSA                half;
            private:
                void *                        z___[ Y_WORDS_FOR(ORDINATE) ];
                const ORDINATE               &zord;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ComputeD2);
            };



        }
    }

}

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
                      const Readable<T>    &aorg,
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
    const size_t        _n1   = sizeof(_t1)/sizeof(_t1[0]);

    static const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    static const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
    const size_t        _n2   = sizeof(_t2)/sizeof(_t2[0]);

    Fit::Sample<double,double>::Pointer S1 = new Fit::LightSample<double,double>("S1",_t1,_x1,_n1);
    Fit::Sample<double,double>::Pointer S2 = new Fit::LightSample<double,double>("S2",_t2,_x2,_n2);

    Fit::HeavySample<double, V2D<double> >    *P1 = new Fit::HeavySample<double, V2D<double> >("H1");
    Fit::Sample<double, V2D<double> >::Pointer H1 = P1;




    P1->add(0.1,  V2D<double>(1,0)  );
    P1->add(1.57, V2D<double>(0,1)  );
    P1->add(3,    V2D<double>(-1,0) );
    P1->add(4.7,  V2D<double>(0,-1) );


    std::cerr << S1 << std::endl;
    std::cerr << S2 << std::endl;
    std::cerr << H1 << std::endl;

    S1->saveDatFile();
    S2->saveDatFile();
    H1->saveDatFile();

    S1->prepare();
    S2->prepare();
    H1->prepare();


    {
        Fit::Variables all;
        all << "t0" << "D1" << "D2";
        std::cerr << "all =" << all << std::endl;

        Fit::Variables var1;
        var1.link( all["t0"] );
        var1.link( "D", all["D1"]);
        std::cerr << "var1=" << var1 << std::endl;

        Fit::Variables var2;
        var2.link( all["t0"] );
        var2.link( "D", all["D2"]);
        std::cerr << "var2=" << var2 << std::endl;



        Vector<double> aorg(all.span(),0);
        std::cerr << "aorg=" << aorg << std::endl;

        all(aorg,"t0") = 100.0;
        var1(aorg,"D") = 0.1;
        var2(aorg,"D") = 0.12;


        all.display( "(all) ", std::cerr, aorg);
        std::cerr << std::endl;
        var1.display("(v1)  ", std::cerr, aorg);
        std::cerr << std::endl;
        var2.display("(v2)  ", std::cerr, aorg);
        std::cerr << std::endl;

        Fit::ComputeD2<double,double> Eval1D;
        F1D<double> f1;
        Fit::ComputeD2<double,double>::OutOfOrderFunc F( &f1, & F1D<double>::F );
        Fit::SequentialWrapper<double, double>        Fw( F );
        Fit::ComputeD2<double,double>::OutOfOrderGrad G( &f1, & F1D<double>::G );

        const double D21  = Eval1D.Of(F,*S1,aorg,var1);
        const double D21w = Eval1D.Of(Fw,*S1,aorg,var1);
        std::cerr << "D21=" << D21 << " / " << D21w << std::endl;

        Vector<double> beta(all.span(),0);
        Vector<bool>   used(all.span(),true);

        const double D21a = Eval1D.Of(F,*S1, aorg, var1, used, G, beta);
        std::cerr << "D21a=" << D21a << std::endl;
        std::cerr << "beta=" << beta << std::endl;

        const double D22a = Eval1D.Of(F,*S2, aorg, var2, used, G, beta);
        std::cerr << "D22a=" << D22a << std::endl;
        std::cerr << "beta=" << beta << std::endl;
    }

    {
        Fit::Variables vars;
        vars << "radius" << "x_c" << "y_c";
        typedef V2D<double> VTX;
        Fit::ComputeD2<double,VTX>                 Eval2D;
        Circle<double>                             Circ;
        Fit::ComputeD2<double,VTX>::OutOfOrderFunc F( &Circ, & Circle<double>::F );
        Fit::ComputeD2<double,VTX>::OutOfOrderGrad G( &Circ, & Circle<double>::G );

        Vector<double> aorg(vars.span(),0);
        vars(aorg,"x_c")    = 0.1;
        vars(aorg,"y_c")    = 0.02;
        vars(aorg,"radius") = 0.78;
        vars.display("", std::cerr, aorg);

        const double D21 = Eval2D.Of(F, *H1, aorg, vars);
        std::cerr << "D21 = " << D21 << std::endl;

        Vector<double> beta(vars.span(),0);
        Vector<bool>   used(vars.span(),true);
        const double   D21a = Eval2D.Of(F,*H1, aorg, vars, used, G, beta);
        std::cerr << "D21a = " << D21a << std::endl;
        std::cerr << "beta = " << beta << std::endl;

    }



}
Y_UDONE()
