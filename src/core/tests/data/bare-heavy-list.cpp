
#include "y/data/small/heavy/list/bare.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_bare_heavy_list)
{

    String hello = "Hello";
    const String world = "World";

    {
        Small::BareHeavyList<String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::BareHeavyList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }

    {
        Small::BareHeavyList<const String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::BareHeavyList<const String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }



}
Y_UDONE()



