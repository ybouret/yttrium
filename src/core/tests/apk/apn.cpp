#include "y/utest/run.hpp"
#include "y/apk/element.hpp"


#include <cstring>

using namespace Yttrium;


namespace Yttrium
{
    namespace APK
    {


      
    }
}

Y_UTEST(apk_n)
{

    for(size_t i=0;i<=20;++i)
    {

        APK::Element el(i);
        std::cerr << i << " => " << el.maxBytes << std::endl;

    }

    Y_SIZEOF(APK::Element);



}
Y_UDONE()

