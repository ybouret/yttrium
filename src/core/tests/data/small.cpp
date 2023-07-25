#include "y/data/small/bare.hpp"
#include "y/data/small/solo.hpp"

#include "y/data/small/heavy-node.hpp"
#include "y/data/small/light-node.hpp"
#include "y/utest/run.hpp"

#include "y/data/list.hpp"
#include "y/data/pool.hpp"
#include "y/memory/out-of-reach.hpp"

#include "y/object.hpp"
#include "y/counted.hpp"

#include "y/string.hpp"

using namespace Yttrium;

namespace Yttrium
{

    

    namespace Small
    {
        template <typename NODE>
        class CoopCache : public Object, public Counted, public SoloCache<NODE>
        {
        public:
            explicit CoopCache() noexcept {}
            virtual ~CoopCache() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CoopCache);
        };

        template <typename NODE>
        class CoopProxy
        {
        public:
            typedef CoopCache<NODE> CacheType;

            explicit CoopProxy() noexcept :
            cache( new CacheType() )
            {
                assert(cache);
            }

            explicit CoopProxy(const CoopProxy &proxy) noexcept : cache(proxy.cache)
            {
                assert(cache);
                cache->withhold();
            }

            virtual ~CoopProxy() noexcept
            {
                if(cache->liberate()) delete cache;
                cache = 0 ;
            }

            CacheType       * operator->() noexcept       { return cache; }
            const CacheType * operator->() const noexcept { return cache; }

        private:
            Y_DISABLE_ASSIGN(CoopProxy);
            CacheType *cache;

        };


    }





}

Y_UTEST(data_small)
{


    typedef Small::LightNode<String>       vLigthStringNode;
    typedef Small::LightNode<const String> cLigthStringNode;

    String       var = "Hello";
    const String cst = "World";

    vLigthStringNode vsn(var);
    cLigthStringNode csn1(var);
    cLigthStringNode csn2(cst);



    ListOf< Small::HeavyNode<int> > heavyList;
    ListOf< Small::LightNode<int> > lightList;
    int i = 2;

    {
        Small::BareProxy< Small::HeavyNode<int> > heavyBareProxy;
        Small::BareProxy< Small::LightNode<int> > lightBareProxy;

        for(size_t k=0;k<10;++k)
        {
            heavyList.pushTail( heavyBareProxy->produce(i) );
            lightList.pushTail( lightBareProxy->produce(i) );
        }

        while(heavyList.size) heavyBareProxy->destroy( heavyList.popTail() );
        while(lightList.size) lightBareProxy->destroy( lightList.popTail() );


        std::cerr << "stowage: " << heavyBareProxy->stowage() << " / " << lightBareProxy->stowage() << std::endl;
    }

    {
        Small::SoloProxy< Small::HeavyNode<int> > heavySoloProxy;
        Small::SoloProxy< Small::LightNode<int> > lightSoloProxy;

        for(size_t k=0;k<10;++k)
        {
            heavyList.pushTail( heavySoloProxy->produce(i) );
            lightList.pushTail( lightSoloProxy->produce(i) );
        }

        while(heavyList.size) heavySoloProxy->destroy( heavyList.popTail() );
        while(lightList.size) lightSoloProxy->destroy( lightList.popTail() );

        std::cerr << "stowage: " << heavySoloProxy->stowage() << " / " << lightSoloProxy->stowage() << std::endl;
    }
    

    {
        Small::CoopProxy< Small::HeavyNode<int> > heavyCoopProxy;
        Small::CoopProxy< Small::LightNode<int> > lightCoopProxy;

        for(size_t k=0;k<10;++k)
        {
            heavyList.pushTail( heavyCoopProxy->produce(i) );
            lightList.pushTail( lightCoopProxy->produce(i) );
        }

        while(heavyList.size) heavyCoopProxy->destroy( heavyList.popTail() );
        while(lightList.size) lightCoopProxy->destroy( lightList.popTail() );

        std::cerr << "stowage: " << heavyCoopProxy->stowage() << " / " << lightCoopProxy->stowage() << std::endl;
    }


}
Y_UDONE()


