
#include "y/mkl/ode/rk4.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/xreal.hpp"

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
}



Y_UTEST(ode_rk4)
{

    makeExp<double>("rk4-exp.dat");
    makeExp< XReal<double> >("rk4-exp-x.dat");


}
Y_UDONE()

