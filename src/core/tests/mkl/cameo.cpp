#include "y/mkl/cameo/static-add.hpp"
#include "y/mkl/cameo/add.hpp"
#include "../main.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(mkl_cameo)
{
    Random::Rand ran;


    {
        Cameo::Add<double> xadd;
        double             vsum = 0;

        for(size_t i=1+ran.leq(20);i>0;--i)
        {
            const double x = Bring<double>::Get(ran);
            xadd << x;
            vsum += x;
        }
        const double xsum = xadd.sum();
        std::cerr << "vsum=" << vsum << std::endl;
        std::cerr << "xsum=" << xsum << std::endl;
    }




    {
        Cameo::Add<double,Memory::Pooled> xadd(13);
        Y_CHECK(xadd.capacity()>=13);

        xadd.push(10);
        xadd.push(2,3.1);
        xadd.push(100);
        xadd.push(-4);
        xadd.push(-50.3);

        std::cerr << xadd << std::endl;

        const double res = xadd.sum();
        std::cerr << "res=" << res << std::endl;
    }

    {
        Cameo::Add<apq,Memory::Dyadic> xadd;


        std::cerr << xadd.sum() << std::endl;

        xadd.push( apq(1,2)  );
        xadd.push( apq(-1,3) );
        xadd.push( apq(1)    );
        std::cerr << xadd.sum() << std::endl;

    }


#if 1
    {

        Cameo::StaticAdd< Complex<float>, 10 > xadd;

        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );
        xadd.push( Bring< Complex<float> >::Get(ran) );

        std::cerr << xadd.sum() << std::endl;

        std::cerr << xadd.sum() << std::endl;

    }
#endif


    {
        Cameo::StaticAdd< XReal<long double>, 100 > xadd;

        for(size_t i=0;i<10;++i)
        {
            xadd << Bring< XReal<long double> >:: Get(ran);
        }

        std::cerr << xadd.sum() << std::endl;

    }


    {
        Cameo::StaticAdd< Complex< XReal<double> > , 13 > xadd;

        for(size_t i=0;i<10;++i)
        {
            xadd << Bring< Complex< XReal<double> > >:: Get(ran);
        }

        std::cerr << xadd.sum() << std::endl;

    }



}
Y_UDONE()

