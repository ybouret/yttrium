#include "y/data/small/light/bare-list.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_bare_light_list)
{

    String hello = "Hello";
    String world = "World";

    {
        Small::BareLightList<String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::BareLightList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }

    {
        Small::BareLightList<const String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::BareLightList<const String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }



}
Y_UDONE()



