

#include "y/data/small/heavy/list/solo.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_solo_heavy_list)
{

    String hello = "Hello";
    const String world = "World";

    {
        Small::SoloHeavyList<String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::SoloHeavyList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }

    {
        Small::SoloHeavyList<const String> L;
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::SoloHeavyList<const String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }



}
Y_UDONE()



