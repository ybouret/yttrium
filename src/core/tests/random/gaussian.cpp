

#include "y/random/gaussian.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

using namespace Yttrium;


Y_UTEST(random_gaussian)
{
    Random::SharedBits sharedBits = new Random::Rand();

    Random::Gaussian<float>       gf(sharedBits);
    Random::Gaussian<double>      gd(sharedBits);
    Random::Gaussian<long double> gl(sharedBits);

    Vector<double> g;

    for(size_t i=1000;i>0;--i) g << gd();
    HeapSort::Call(g,Comparison::IncreasingAbs<double>);
    //std::cerr << g << std::endl;
    double ave = 0;
    for(size_t i=1;i<=g.size();++i) ave += g[i];
    ave /= g.size();
    std::cerr << "ave=" << ave << std::endl;
    for(size_t i=g.size();i>0;--i) g[i] = Squared( g[i] - ave );
    HeapSort::Call(g,Comparison::Increasing<double>);
    double var = 0;
    for(size_t i=1;i<=g.size();++i)
    {
        var += g[i];
    }
    var /= g.size();
    const double sig = std::sqrt(var);
    std::cerr << "sig=" << sig << std::endl;

}
Y_UDONE()

