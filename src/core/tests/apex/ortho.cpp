

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
        Ortho::QVector::Cache cache(dims);
        Ortho::QVector::List  qlist;
        Ortho::QVector *v = qlist.pushTail( cache.query() );
        Y_ASSERT(dims==v->size());
        Y_ASSERT(dims==v->dimensions);

        Vector<Rational> q(dims,0);
        

    }

}
Y_UDONE()
