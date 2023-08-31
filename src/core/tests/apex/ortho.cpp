
#include "y/apex/ortho/vector.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_ortho)
{
    Random::Rand ran;

    Vector<int> coef;
    coef << -1 << 2 << 0;

    Apex::Ortho::Vector V(coef,AsCopy);

    std::cerr << "coef=" << coef << std::endl;
    std::cerr << "V   =" << V << std::endl;

    
    Vector<apq> tmp(V.size(),0);
    tmp[1] = 1;
    std::cerr << tmp << std::endl;
    V.computeOrtho(tmp);
    std::cerr << "Q=" << tmp << std::endl;

}
Y_UDONE()


