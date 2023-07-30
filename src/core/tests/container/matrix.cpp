
#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/iterator/display-all.hpp"
#include "y/string.hpp"
#include "y/apex/natural.hpp"
#include <cstring>

using namespace Yttrium;



Y_UTEST(container_matrix)
{

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

    //Matrix<double> m0;
    //Matrix<double> m(2,4);
    Matrix<String> s(8,7);

    {
        Matrix<apn,Memory::Dyadic>      I(5,3);
        Matrix<apn,Memory::Dyadic>      A(I);
    }

    {
        Matrix<unsigned,Memory::Dyadic> u(5,3);
        Matrix<apn,Memory::Pooled>      A(u,Identity<unsigned>);
        std::cerr << A << std::endl;
    }


}
Y_UDONE()

