
#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/iterator/display-all.hpp"
#include "../main.hpp"
#include <cstring>

using namespace Yttrium;



Y_UTEST(container_matrix)
{
    Random::Rand ran;
    int arr[4] = {1,2,3,4};

    LightArray<int> Arr(arr,4);
    Y_CHECK( 0 == strcmp(Core::LightArray::CallSign,Arr.callSign()) );
    Iterating::DisplayAll::Of(Arr);

#if 1
    MatrixRow<int> row(arr,4);
    Y_CHECK( 0 == strcmp(Core::MatrixRow::CallSign,row.callSign()) );
    Iterating::DisplayAll::Of(row);
#endif

    Y_SIZEOF(MatrixRow<int>);
    Y_SIZEOF(MatrixRow<String>);

    for(size_t r=1;r<=3;++r)
    {
        for(size_t c=1;c<=3;++c)
        {
            Matrix<int> M(r,c);
            for(size_t i=1;i<=r;++i)
            {
                for(size_t j=1;j<=c;++j)
                {
                    M[i][j] = int(i+j);
                }
            }
            std::cerr << "M" << r << "x" << c << " = " << M << std::endl;
        }
    }

    
    //Matrix<double> m0;
    //Matrix<double> m(2,4);
    Matrix<String> s(8,7);

    {
        Matrix<apn,Memory::Dyadic>      I(5,3);
        Matrix<apn,Memory::Dyadic>      A(I);
    }

    {
        Matrix<uint16_t,Memory::Dyadic> u(5,3);
        FillMatrix(u,ran);
        Matrix<apn,Memory::Pooled>      A(CopyOf,u,Identity<unsigned>);
        std::cerr << "A0=" << A << std::endl;
        A.swapRows(1,5);
        std::cerr << "A1=" << A << std::endl;

        Matrix<apq,Memory::Dyadic> M(A.rows-1,A.cols-1);
        for(size_t i=1;i<=A.rows;++i)
        {
            for(size_t j=1;j<=A.cols;++j)
            {
                A.buildMinor(M,i,j);
                std::cerr << "M" << i << "_" << j << "=" << M << std::endl;
            }
        }

        Matrix<apq,Memory::Pooled> At(TransposeOf,A);
        std::cerr << "A="  << A << std::endl;
        std::cerr << "At=" << At << std::endl;

    }


    {
        Matrix<String> M;
        M.make(4,4);
        //M.make(5,5);
    }

}
Y_UDONE()

