#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/api.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/indexing.hpp"
#include "y/mkl/antelope/add.hpp"
#include "y/stream/libc/output.hpp"
#include "y/memory/buffer/of.hpp"
#include "y/mkl/opt/minimize.hpp"

using namespace Yttrium;

namespace {

    class Sum
    {
    public:
        Sum(const size_t num, Random::Bits &ran) :
        n(num),
        a(n,0),
        b(n,0),
        z(n,0),
        xadd(n)
        {
            for(size_t i=n;i>0;--i)
            {
                {
                    double A = 1 + 4 * ran.to<double>();
                    if(ran.choice()) A = -A;
                    Coerce(a[i]) = A;
                }
                Coerce(b[i]) = 8 * ran.to<double>();
                Coerce(z[i]) = -b[i]/a[i];
            }


            Vector<size_t> idx(n,0);
            Indexing::Make(idx, Comparison::Increasing<double>, z);
            Memory::BufferOf<double> buf(n);
            Indexing::Rank(Coerce(z),idx, buf.rw_addr());
            Indexing::Rank(Coerce(a),idx, buf.rw_addr());
            Indexing::Rank(Coerce(b),idx, buf.rw_addr());
            std::cerr << "a=" << a << std::endl;
            std::cerr << "b=" << b << std::endl;
            std::cerr << "z=" << z << std::endl;

        }

        double operator()(const double x)
        {
            xadd.free();
            for(size_t i=n;i>0;--i) xadd << fabs(a[i]*x+b[i]);
            return xadd.sum();
        }

        ~Sum() noexcept {}

        const size_t         n;
        const Vector<double> a;
        const Vector<double> b;
        const Vector<double> z;
        MKL::Antelope::Add<double> xadd;
    };

}

#include "y/text/ascii/convert.hpp"

Y_UTEST(norm)
{
    Random::ParkMiller ran;
    size_t             n = 2;
    if(argc>1)
    {
        n = ASCII::Convert::To<size_t>(argv[1],"n");
    }


    Sum sum(n,ran);

    MKL::Triplet<double> xx = { sum.z.head()-1, sum.z[1+sum.z.size()/2], sum.z.tail()+1 };
    MKL::Triplet<double> ff = { sum(xx.a), sum(xx.b), sum(xx.c) };

    const double xopt = MKL::Minimize<double>::Locate(MKL::Minimizing::Direct, sum, xx, ff);
    std::cerr << "xopt=" << xopt << std::endl;

    {
        OutputFile fp("norm.dat");
        for(double x=-10;x<=10;x+=0.01)
        {
            fp("%g %g\n",x,sum(x));
        }
    }






}
Y_UDONE()
