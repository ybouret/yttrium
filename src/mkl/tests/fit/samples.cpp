

#include "y/mkl/fit/sequential.hpp"
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
            class RegularFunction : public Object, public Counted
            {
            public:
                typedef ArcPtr<RegularFunction> Handle;;

                inline virtual ~RegularFunction() noexcept {}

                virtual ORDINATE operator()(const ABSCISSA           &,
                                            const Readable<ABSCISSA> &,
                                            const Variables          &) = 0;

            protected:
                inline explicit RegularFunction() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RegularFunction);
            };

            template <typename ABSCISSA, typename ORDINATE>
            class SequentialWrapper : public Sequential<ABSCISSA,ORDINATE>
            {
            public:
                typedef TypeFor<ABSCISSA,ORDINATE>           MyType;
                typedef Sequential<ABSCISSA,ORDINATE>        SequentialFunc;
                typedef typename MyType::OutOfOrderFunc      OutOfOrderFunc;

                inline explicit SequentialWrapper(const OutOfOrderFunc &h) : SequentialFunc(), func(h) {}
                inline virtual ~SequentialWrapper() noexcept {}

                OutOfOrderFunc func;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SequentialWrapper);

                virtual ORDINATE init(const ABSCISSA           &abs0,
                                      const Readable<ABSCISSA> &aorg,
                                      const Variables          &vars)
                {
                    return func(abs0,aorg,vars);
                }

                virtual ORDINATE move(const ABSCISSA           &abs0,
                                      const ORDINATE           &ord0,
                                      const ABSCISSA           &abs1,
                                      const Readable<ABSCISSA> &aorg,
                                      const Variables          &vars)
                {
                    (void)abs0;
                    (void)ord0;
                    return func(abs1,aorg,vars);
                }

            };

            template <typename ABSCISSA, typename ORDINATE>
            class ComputeD2
            {
            public:
                typedef Sample<ABSCISSA,ORDINATE>      SampleType;
                typedef TypeFor<ABSCISSA,ORDINATE>     MyType;

                typedef typename SampleType::Abscissae Abscissae;
                typedef typename SampleType::Ordinates Ordinates;;
                typedef TL3(ABSCISSA,const Readable<ABSCISSA>&,const Variables) OutOfOrderArgs;
                typedef Functor<ORDINATE,OutOfOrderArgs> OutOfOrderFunc;
                typedef Sequential<ABSCISSA,ORDINATE>    SequentialFunc;

                static const size_t                   Dimension = SampleType::Dimension;

                Antelope::Add<ABSCISSA> xadd;

                explicit ComputeD2() : half(0.5)
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

                    // second point
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

                const ABSCISSA half;
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

        virtual T compute(const T              &t,
                          const Readable<T>    &aorg,
                          const Fit::Variables &vars)
        {
            const T t0 = vars(aorg,"t0");
            const T D  = vars(aorg,"D");
            if(t<=t0) return 0;
            return sqrt( D*(t-t0) );
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(F1D);
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
    Fit::ComputeD2<double,double>::OutOfOrderFunc F1( &f1, & F1D<double>::compute );
    Fit::SequentialWrapper<double, double>        F1w( F1 );

    const double D21  = Eval1D.Of(F1,*S1,aorg,var1);
    const double D21w = Eval1D.Of(F1w,*S1,aorg,var1);
    std::cerr << "D21=" << D21 << " / " << D21w << std::endl;

}
Y_UDONE()
