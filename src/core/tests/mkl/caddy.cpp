#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/caddy.hpp"
#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T> static inline
    void TestCaddy(Random::Bits &ran)
    {
        typedef typename Antelope::Caddy<T>::XNode XNode;
        Antelope::Caddy<T> caddy;
        Matrix<T>          M(4+ran.leq(4),10+ran.leq(10));
        FillMatrix(M,ran);
        std::cerr << "M=" << M << std::endl;
        const size_t numVars = M.rows;
        const size_t numData = M.cols;

        caddy.setup(numVars,numData);


        for(size_t j=numData;j>0;--j)
        {
            XNode *node=caddy.head;
            for(size_t i=1;i<=numVars;++i,node=node->next)
            {
                (*node) << M[i][j];
            }
        }

        CxxArray<T> sumF(numVars);
        caddy.sum(sumF);
        std::cerr << "sumF=" << sumF << std::endl;
        


    }
}

Y_UTEST(mkl_caddy)
{

    Random::Rand ran;
    TestCaddy<apz>(ran);
    TestCaddy<float>(ran);
    TestCaddy<double>(ran);
    TestCaddy<long double>(ran);
    TestCaddy< XReal<float> >(ran);
    TestCaddy< XReal<double> >(ran);
    TestCaddy< XReal<long double> >(ran);


}
Y_UDONE()

