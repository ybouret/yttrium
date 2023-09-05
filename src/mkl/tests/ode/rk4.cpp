
#include "y/mkl/ode/rk4.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/api.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    class dExp
    {
    public:
        explicit dExp(const T fac) noexcept : k(fac) {}
        virtual ~dExp() noexcept {}

        const T k;

        inline void compute(Writable<T> &dydt, double, const Readable<T> &y)
        {
            dydt[1] = k * y[1];
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(dExp);
    };


    template <typename T>
    static inline
    void makeExp(const char *fileName)
    {
        dExp<T> eq(-1.2);
        typename ODE::Field<T>::Equation dexp(&eq, & dExp<T>::compute);

        Vector<T> y(1,2.7);
        const double tmax = 8;
        const size_t N    = 1000;
        ODE::RK4<T>  rk4;

        Libc::OutputFile fp(fileName);
        fp("%g %g\n",0.0,double(y[1]));
        for(size_t i=1;i<=N;++i)
        {
            const double t0 = ((i-1)*tmax)/N;
            const double t1 = (i*tmax)/N;
            rk4(y,dexp,t0,t1);
            fp("%g %g\n",t1,double(y[1]));

        }
    }

    template <typename T>
    class dCos
    {
    public:
        dCos() noexcept {}
        ~dCos() noexcept {}

        inline void compute(Writable<T> &dydt, double, const Readable<T> &y)
        {
            dydt[1] =  y[2];
            dydt[2] = -y[1];
        }

        
        inline void callback(double, Writable<T> &y)
        {
            const T fac = Hypotenuse(y[1],y[2]);
            y[1] /= fac;
            y[2] /= fac;
        }
    };

    template <typename T>
    static inline
    void makeCos(const char *fileName)
    {
        dCos<T> eq;
        typename ODE::Field<T>::Equation dcos(&eq, & dCos<T>::compute);
        typename ODE::Field<T>::Callback fcos(&eq, & dCos<T>::callback);

        Vector<T> y(2,0);
        y[1] = 1;
        y[2] = 0;
        const double tmax = 80;
        const size_t N    = 10000;
        ODE::RK4<T>  rk4;

        Libc::OutputFile fp(fileName);
        fp("%g %g %g\n",0.0,double(y[1]),double(y[2]));
        for(size_t i=1;i<=N;++i)
        {
            const double t0 = ((i-1)*tmax)/N;
            const double t1 = (i*tmax)/N;
            rk4(y,dcos,t0,t1,&fcos);
            fp("%g %g %g\n",t1,double(y[1]),double(y[2]));

        }
    }

}



Y_UTEST(ode_rk4)
{

    makeExp<double>("rk4-exp.dat");
    makeExp< XReal<double> >("rk4-exp-x.dat");

    makeCos<double>("rk4-cos.dat");


}
Y_UDONE()

