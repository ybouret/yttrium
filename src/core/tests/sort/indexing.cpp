
#include "y/sort/indexing.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/sequence/vector.hpp"
#include "y/comparison.hpp"

using namespace Yttrium;

Y_UTEST(sort_indexing)
{

    Random::Rand ran;

    {
        Vector<double> data;
        Vector<size_t> indx;

        for(size_t i=2 + ran.leq(10);i>0;--i)
        {
            data << ran.symm<double>();
            indx << 0;
        }

        Indexing::Make(indx, Comparison::Increasing<double>, data);

        std::cerr << "data : " << data << std::endl;
        std::cerr << "indx : " << indx << std::endl;
    }

    {
        const size_t n       = 5;
        double       data[n] = { 0 };
        size_t       indx[n] = { 0 };
        for(size_t i=0;i<n;++i) data[i] = ran.symm<double>();
        Indexing::Tableau(indx,n, Comparison::Increasing<double>,data);
        Core::Display(std::cerr << "data = ",data, n) << std::endl;
        Core::Display(std::cerr << "indx = ",indx, n) << std::endl;

    }

}
Y_UDONE()
