

#include "y/data/small/light/list/coop.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_coop_light_list)
{

    String hello = "Hello";
    String world = "World";

    Small::CoopLightList<String>::ProxyType px;
    {
        Small::CoopLightList<String> L(px);
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::CoopLightList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.free();
        std::cerr << L.proxy->stowage() << std::endl;
    }
    std::cerr << px->stowage() << std::endl;




}
Y_UDONE()



