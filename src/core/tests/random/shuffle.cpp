
#include "y/random/shuffle.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;



Y_UTEST(random_shuffle)
{
    Random::Rand ran;

    int arr[] = { 1, 2, 3, 4, 5};
    
    Core::Display(std::cerr << "arr=", arr, Y_STATIC_SIZE(arr)) << std::endl;
    Random::Shuffle::Tableau(arr,Y_STATIC_SIZE(arr),sizeof(arr[0]),ran);
    Core::Display(std::cerr << "arr=", arr, Y_STATIC_SIZE(arr)) << std::endl;


}
Y_UDONE()

