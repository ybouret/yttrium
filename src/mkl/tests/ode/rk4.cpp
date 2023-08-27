
#include "y/mkl/ode/rk4.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    class dExp
    {
    public:
        explicit dExp(const double fac) noexcept : k(fac) {}
        virtual ~dExp() noexcept {}

        const double k;

        inline void compute(Writable<double> &dydt, double, const Readable<double> &y)
        {
            dydt[1] = k * y[1];
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(dExp);
    };
}

Y_UTEST(ode_rk4)
{

    dExp eq(-1.2);
    ODE::Field<double>::Equation dexp(&eq, & dExp::compute);

    Vector<double> y(1,2.7);
    const double tmax = 4;
    const size_t N    = 100;
    ODE::RK4<double> rk4;

    Libc::OutputFile fp("rk4-exp.dat");
    fp("%g %g\n",0.0,y[1]);
    for(size_t i=1;i<=N;++i)
    {
        const double t0 = ((i-1)*tmax)/N;
        const double t1 = (i*tmax)/N;
        rk4(y,dexp,t0,t1);
        fp("%g %g\n",t1,y[1]);

    }



}
Y_UDONE()

