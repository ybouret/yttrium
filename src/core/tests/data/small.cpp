#include "y/data/small/proxy/bare.hpp"
#include "y/data/small/proxy/coop.hpp"

#include "y/data/small/heavy/node.hpp"
#include "y/data/small/light/node.hpp"

#include "y/data/small/proto/list.hpp"
#include "y/data/small/proto/pool.hpp"

#include "y/data/small/light/bare-list.hpp"
#include "y/data/small/light/solo-list.hpp"
#include "y/data/small/light/coop-list.hpp"

#include "y/data/small/heavy/list.hpp"


#include "y/utest/run.hpp"

#include "y/memory/out-of-reach.hpp"


#include "y/string.hpp"

using namespace Yttrium;


Y_UTEST(data_small)
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

    Small::CoopLightList<String>::ProxyType px;
    {
        Small::CoopLightList<String> L(px);
        L << hello << world << hello;
        std::cerr << "L=" << L << std::endl;
        {
            const Small::CoopLightList<String> tmp(L);
            std::cerr << " =" << tmp << std::endl;
        }
        L.release();
        std::cerr << L.proxy->stowage() << std::endl;
    }
    std::cerr << px->stowage() << std::endl;
    
}
Y_UDONE()


