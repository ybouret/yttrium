
#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/iterator/display-all.hpp"
#include "y/string.hpp"

using namespace Yttrium;



Y_UTEST(container_matrix)
{

    int arr[4] = {1,2,3,4};

    MatrixRow<int> row(arr,4);
    Iterating::DisplayAll::Of(row);

    Y_SIZEOF(MatrixRow<int>);
    Y_SIZEOF(MatrixRow<String>);

    Matrix<double> m0;
    Matrix<double> m(2,4);


}
Y_UDONE()

