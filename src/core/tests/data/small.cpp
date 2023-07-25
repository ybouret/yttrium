#include "y/data/small/proxy/bare.hpp"
#include "y/data/small/proxy/coop.hpp"

#include "y/data/small/heavy/node.hpp"
#include "y/data/small/light/node.hpp"

#include "y/data/small/proto/list.hpp"
#include "y/data/small/proto/pool.hpp"


#include "y/utest/run.hpp"

#include "y/memory/out-of-reach.hpp"


#include "y/string.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Small
    {
        template <typename T, template <typename> class PROXY>
        class HeavyList : public ProtoList< HeavyNode<T>, PROXY>
        {
        public:
            typedef HeavyNode<T>              NodeType;
            typedef ProtoList<NodeType,PROXY> ProtoType;
            typedef PROXY<NodeType>           ProxyType;

            inline explicit HeavyList() : ProtoType() {}
            inline explicit HeavyList(const ProxyType &_) noexcept : ProtoType(_) {}
            inline explicit HeavyList(const HeavyList &_) : ProtoType(_) {}
            inline virtual ~HeavyList() noexcept {}

        private:
            Y_DISABLE_ASSIGN(HeavyList);
        };

        template <typename T>
        class BareHeavyList : public HeavyList<T,BareProxy>
        {
        public:
            typedef HeavyList<T,BareProxy> ListType;
            typedef HeavyNode<T>           NodeType;

            inline explicit BareHeavyList() noexcept : ListType() {}
            inline virtual ~BareHeavyList() noexcept {}
            inline BareHeavyList(const BareHeavyList &_) : ListType(_) {}

        private:
            Y_DISABLE_ASSIGN(BareHeavyList);
        };

    }
}

Y_UTEST(data_small)
{

#if 0
    typedef Small::LightNode<String> LightNode;
    typedef Small::ProtoList<LightNode,Small::BareProxy> LightList;
    typedef Small::ProtoPool<LightNode,Small::BareProxy> LightPool;

    String       hello = "Hello";
    String       world = "World";

    {
        LightList L;

        L.pushTail(  L.proxy->produce(hello) );
        L.pushTail(  L.proxy->produce(world) );

        std::cerr << "L=" << L << std::endl;

        {
            const LightList tmp(L);
            std::cerr << tmp << std::endl;
        }
    }

    {
        LightPool P;
        P.store( P.proxy->produce(world) );
        P.store( P.proxy->produce(hello) );
        std::cerr << "P=" << P << std::endl;
        {
            const LightPool Q(P);
            std::cerr << "Q=" << Q << std::endl;
        }
    }
#endif





}
Y_UDONE()


