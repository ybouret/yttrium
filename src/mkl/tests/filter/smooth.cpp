#include "y/mkl/filter/selector.hpp"
#include "y/random/bits.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/filter/smooth.hpp"

#include <cmath>

using namespace Yttrium;


Y_UTEST(filter_smooth)
{

    Random::Rand ran;
    
    // generate some data
    Vector<double> X,Y;
    {
        const size_t np=100;
        Libc::OutputFile fp("smooth-data.dat");
        double xx = 0;
        for(size_t i=0;i<=np;++i)
        {
            xx += 0.05 + 0.05 * ran.to<double>();
            X << xx;
            Y << cos(xx);
            fp("%.15g %.15g\n", X.tail(), Y.tail());
        }
    }

    MKL::Smooth<double> sm;

    sm.reserveMaxDegree(5);

    Vector<double> xs, ys;



    const double   xmin  = 0;
    const double   xmax  = X.tail()*1.01;



    {
        Libc::OutputFile fp("smooth.dat");
        const size_t NP = 20;
        for(size_t i=0;i<=NP;++i)
        {
            const double xx = xmin + i*(xmax-xmin)/double(NP);
            std::cerr << "xx=" << xx << std::endl;
            MKL::Selector::Fill(xx, 0.5, 0.7, xs, ys, X, Y);

            sm(xx,xs,ys,5);
            fp("%.15g %.15g %.15g %.15g\n", xx, sm[1], sm[2], sm[3] );


#if 0
            if(!sm.prepare(xx, xs, ys,5)) throw Exception("singular smooth");

            if(false)
            {
                Libc::OutputFile fps("select.dat");
                for(size_t j=1;j<=xs.size();++j)
                {
                    fps("%.15g %.15g\n", xs[j], ys[j]);
                }
                fps << '\n';
                fps("%.15g %.15g\n", xx, sm.code->yOffset);
                fps("%.15g %.15g\n", xx, 0.0);

            }

            fp("%.15g %.15g\n", xx, sm[0] );
#endif
        }
    }


    std::cerr << "plot 'smooth-data.dat' w lp, 'smooth.dat' w lp, 'smooth.dat' u 1:3  w lp, -sin(x), 'smooth.dat' u ($1):(-$4) w lp" << std::endl;




}
Y_UDONE()

