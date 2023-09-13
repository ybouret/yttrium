
#include "y/mkl/algebra/tridiag.hpp"
#include "y/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "y/container/cxx-array.hpp"
#include "y/system/rtti.hpp"

using namespace Yttrium;

Y_UTEST(algebra_tridiag)
{

    MKL::TriDiag<float> tr(6);

    for(size_t i=1;i<=tr.size();++i)
    {
        tr.a[i] = -float(i);
        tr.b[i] = 1.0f/(i+i);
        tr.c[i] = float(i);
    }

    std::cerr << tr.a << std::endl;
    std::cerr << tr.b << std::endl;
    std::cerr << tr.c << std::endl;

    Matrix<float> M(6,6);
    for(size_t i=1;i<=6;++i)
    {
        for(size_t j=1;j<=6;++j)
        {
            M[i][j] = tr(i,j);
        }
    }
    std::cerr << M << std::endl;




}
Y_UDONE()
