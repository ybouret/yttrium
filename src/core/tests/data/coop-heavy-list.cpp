


#include "y/data/small/heavy/list/coop.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_coop_heavy_list)
{

    String hello = "Hello";
    const String world = "World";

    Small::CoopHeavyList<String>::ProxyType px;
    {
        Small::CoopHeavyList<String> L(px);
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::CoopHeavyList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.free();
        std::cerr << L.proxy->stowage() << std::endl;
    }
    std::cerr << px->stowage() << std::endl;




}
Y_UDONE()



