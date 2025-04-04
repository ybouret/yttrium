

#include "y/mkl/opt/bracket.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include "y/mkl/xreal.hpp"
#include <cmath>

using namespace Yttrium;
using namespace MKL;

 

static inline double F(double x)
{
    return  0.7-cos(0.9*x-0.1);
}

typedef XReal<long double> xreal_t;

static inline xreal_t G(xreal_t x)
{
    return  x+0.01L*x*x;
}



Y_UTEST(opt_bracket)
{
    Bracketing::Verbose = true;

    {
        Triplet<xreal_t> x = { 0,0,1 };
        Triplet<xreal_t> g = { G(x.a), 0, G(x.c) };

        if( Bracket<xreal_t>::Inside(G,x,g) )
        {
            std::cerr << x << " -> " << g << std::endl;
        }
        else
        {
            std::cerr << "@" << x.b << ": " << g.b << std::endl;
        }
    }



    {
        Triplet<double> x = { 0, 0, 0.5 };

        if(argc>1)
        {
            x.a = atof(argv[1]);

        }

        Triplet<double> f = { F(x.a), 0, F(x.c) };


        if( Bracket<double>::Inside(F,x,f) )
        {
            std::cerr << x << " -> " << f << std::endl;
        }
        else
        {
            std::cerr << "@" << x.b << ": " << f.b << std::endl;
        }
    }



}
Y_UDONE()
