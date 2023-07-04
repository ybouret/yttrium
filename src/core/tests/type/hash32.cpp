
#include "y/type/hash32.hpp"
#include "y/data/list.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace
{
    struct XNode
    {
        XNode *next;
        XNode *prev;

    };

}

Y_UTEST(type_hash32)
{
    for(uint32_t i=1;i<=16;++i)
    {
        const uint32_t n = i*4;
        const uint32_t h = Core::Hash32(i);
        std::cerr << n << " => " << (h&127) << std::endl;
    }

    Y_SIZEOF(ListOf<XNode>);
}
Y_UDONE()

