
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

    template <typename T, typename U> static inline
    void test1D(U * const t1, U * const x1, const size_t n1,
                U * const t2, U * const x2, const size_t n2)
    {
        // declaring samples
        Fit::Samples<T,T>  samples;
        Fit::Sample<T,T> & S1 = samples( new Fit::HeavySample<T,T>("S1", t1, x1, n1) );
        Fit::Sample<T,T> & S2 = samples( new Fit::HeavySample<T,T>("S2", t2, x2, n2) );
        std::cerr << "S1=" << S1.numPoints() << std::endl;
        std::cerr << "S2=" << S2.numPoints() << std::endl;

        // declaring all variables
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


    }

}

Y_UTEST(fit_1d)
{
    static const double _t1[] = { 30,80,140,200,270,320,430,550,640,720,830,890 };
    static const double _x1[] = { 4.414520,5.011710,5.632319,6.194379,6.721311,7.330211,8.009368,8.735363,9.297424,9.707260,10.339578,10.878220};
    static const size_t _n1   = sizeof(_t1)/sizeof(_t1[0]);
    double              _z1[_n1] = { 0 };

    static const double _t2[] = { 60,120,200,270,360,460,580,670,790,920,1040 };
    static const double _x2[] = { 5.199063,5.854801,6.662763,7.365340,8.067916,8.782201,9.578454,10.175644,10.878220,11.651054,12.213115};
    static const size_t _n2   = sizeof(_t2)/sizeof(_t2[0]);
    double              _z2[_n2] = { 0 };

    {
        Fit::Samples<double,double>  samples;
        Fit::Sample<double,double> & S1 = samples( new Fit::LightSample<double,double>("S1",_t1,_x1, _z1, _n1) );
        Fit::Sample<double,double> & S2 = samples( new Fit::LightSample<double,double>("S2",_t2,_x2, _z2, _n2) );
        S1.saveDatFile();
        S2.saveDatFile();
    }

    test1D<float>(_t1,_x1,_n1,_t2,_x2,_n2);
    test1D< XReal<float> >(_t1,_x1,_n1,_t2,_x2,_n2);



}
Y_UDONE()
