
#include "y/data/small/light/solo-list.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_solo_light_list)
{

    String hello = "Hello";
    String world = "World";

    {
        Small::SoloLightList<String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::SoloLightList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }

    {
        Small::SoloLightList<const String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::SoloLightList<const String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }



}
Y_UDONE()



