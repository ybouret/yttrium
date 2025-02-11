

#include "y/apex/api/ortho/vector.hpp"
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
        const Ortho::Metrics  metrics(dims);
        Ortho::QCache         cache = new Ortho::QVector::Cache(metrics);
        Ortho::QVector::List  qlist;
        Ortho::QVector *v = qlist.pushTail( cache->query() );
        Y_ASSERT(dims==v->size());
        Y_ASSERT(dims==v->dimensions);

        Vector<Rational> q(dims,0);

        for(size_t i=dims;i>0;--i)
        {
            q[i] = Rational(ran,5,5);
        }

        std::cerr << "q=" << q << std::endl;
        v->set(q);
        std::cerr << "q=" << q  << std::endl;
        std::cerr << "v=" << *v << std::endl;
    }

}
Y_UDONE()
