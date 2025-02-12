

#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_ortho)
{

    Random::ParkMiller ran;
    for(size_t dims=1;dims<=5;++dims)
    {
        std::cerr << std::endl << "dims=" << dims << std::endl;
        const Ortho::Metrics  metrics(dims);
        Ortho::QCache         cache = new Ortho::QVector::Cache(metrics);
        Ortho::QVector::List  qlist;
        Ortho::QVector *v = qlist.pushTail( cache->query() );
        Y_ASSERT(dims==v->size());
        Y_ASSERT(dims==v->dimensions);

        CxxArray<int> V(dims);
        for(size_t i=dims;i>0;--i) V[i] = ran.in<int>(-5,5);
        v->set(V);
        std::cerr << "V=" << V  << std::endl;
        std::cerr << "v=" << *v << std::endl;

    }

}
Y_UDONE()
