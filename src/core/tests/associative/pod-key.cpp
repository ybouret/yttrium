#include "y/associative/pod-key.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/v2d.hpp"

using namespace Yttrium;


Y_UTEST(associative_pod_key)
{
    typedef POD_Key< V2D<int> > V2Key;

    V2Key v1(CopyOf,1,2);

    v1.displayHexadecimal(std::cerr) << std::endl;
    V2Key v2(v1);
    Y_CHECK(v1==v2);

}
Y_UDONE()

