#include "y/apex/api/univocal.hpp"
#include "y/apex/api/count-non-zero.hpp"

#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


template <typename T>
static inline void ProcessUnivocal(Writable<T> &v)
{
    (std::cerr << "v=" << v).flush();
    const Natural nrm2  = Univocal::Make(v);
    const size_t  nzero = CountNonZero(v);
    std::cerr << " => " << v << " @" << nrm2 << " #" << nzero << std::endl;
    switch(nzero)
    {
        case 0:  Y_ASSERT(0==nrm2); break;
        case 1:  Y_ASSERT(1==nrm2); break;
        default: Y_ASSERT(nrm2>1);  break;
    }
}

Y_UTEST(apex_uni)
{
    Random::ParkMiller ran;

    std::cerr << std::endl << "unsigned" << std::endl;
    {
        for(size_t dim=0;dim<=6;++dim)
        {
            Vector<unsigned> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = 0;
                    if(ran.choice()) v[i] = ran.in<unsigned>(1,10);
                }

                ProcessUnivocal(v);
            }
        }
    }


    std::cerr << std::endl << "signed" << std::endl;
    {
        for(size_t dim=0;dim<=6;++dim)
        {
            Vector<int32_t> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = 0;
                    if(ran.choice()) v[i] = ran.in<int32_t>(-10,10);
                }
                ProcessUnivocal(v);
            }
        }
    }

    std::cerr << std::endl << "Natural" << std::endl;
    {
        for(size_t dim=0;dim<=6;++dim)
        {
            Vector<Natural> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = 0;
                    if(ran.choice()) v[i] = ran.in<natural_t>(1,10);
                }
                ProcessUnivocal(v);
            }
        }
    }


    std::cerr << std::endl << "Integer" << std::endl;
    {
        for(size_t dim=0;dim<=6;++dim)
        {
            Vector<Integer> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = 0;
                    if(ran.choice()) v[i] = ran.in<integer_t>(-10,10);
                }
                ProcessUnivocal(v);
            }
        }
    }

    std::cerr << std::endl << "Rational" << std::endl;
    {
        for(size_t dim=0;dim<=6;++dim)
        {
            Vector<Rational> v(dim,0);
            for(size_t cycle=0;cycle<=10;++cycle)
            {
                for(size_t i=1;i<=dim;++i)
                {
                    v[i] = Rational(ran,5,5);
                    if(ran.to<float>()>0.8) v[i] = 0;
                    if(ran.choice()) v[i] = -v[i];
                }
                ProcessUnivocal(v);
            }
        }
    }

}
Y_UDONE()




