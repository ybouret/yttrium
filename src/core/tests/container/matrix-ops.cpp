
#include "y/container/matrix/ops.hpp"
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/container/iterator/display-all.hpp"
#include "../main.hpp"
#include <cstring>
using namespace Yttrium;


template <typename T> static inline
void MatOps( Random::Bits &ran )
{
    const size_t n = 4;
    MKL::Antelope::Add<T> xadd;
    Vector<T> target;
    Vector<T> source;
    Matrix<T> M;
    for(size_t r=1;r<=n;++r)
    {
        target.adjust(r,0);
        target.ld(0);
        for(size_t c=1;c<=n;++c)
        {
            M.make(r,c);
            source.adjust(c,0);
            FillMatrix(M,ran);
            FillWritable(source,ran);
            MatrixOps::Mul(target,M, source,xadd);
            std::cerr << "M=" << M << std::endl;
            std::cerr << "source=" << source << std::endl;
            std::cerr << "target=" << target << std::endl;
        }
    }
}

Y_UTEST(container_matrix_ops)
{
    Random::Rand ran;

    MatOps<float>(ran);


}
Y_UDONE()
