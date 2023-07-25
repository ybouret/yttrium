#include "y/data/small/supply.hpp"
#include "y/data/small/heavy-node.hpp"
#include "y/data/small/light-node.hpp"
#include "y/data/small/inventory.hpp"
#include "y/utest/run.hpp"

#include "y/data/list.hpp"
#include "y/data/pool.hpp"
#include "y/memory/out-of-reach.hpp"

#include "y/object.hpp"
#include "y/counted.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Small
    {

        

        template <typename T, template <typename> class NODE>
        class BareCache : public Supply
        {
        public:
            Y_ARGS_EXPOSE(T,Type);
            typedef NODE<T> NodeType;

            explicit BareCache() noexcept {}
            virtual ~BareCache() noexcept {}


            inline virtual size_t stowage() const noexcept { return 0; }
            
            inline NodeType *produce(Type &args)
            {
                NodeType *node = Object::zacquire<NodeType>();
                try {
                    return new (node) NodeType(args);
                }
                catch(...)
                {
                    Object::zrelease(node);
                    throw;
                }
            }


            inline virtual void release() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(BareCache);
            virtual void *getFlat() { return Object::zacquire<NodeType>(); }
            virtual void  putFlat(void *blockAddr) noexcept { Object::operator delete(blockAddr,sizeof(NodeType)); }
        };

        template <typename T, template <typename> class NODE>
        class BareProxy
        {
        public:
            explicit BareProxy() noexcept {}
            virtual ~BareProxy() noexcept {}
            BareProxy(const BareProxy &) noexcept {}

            typedef BareCache<T,NODE> CacheType;
            CacheType       * operator->() noexcept       { return &cache; }
            const CacheType * operator->() const noexcept { return &cache; }
        private:
            Y_DISABLE_ASSIGN(BareProxy);
            CacheType cache;
        };


    }

    namespace Small
    {


        template <typename T, template <typename> class NODE>
        class SoloCache
        {
        public:
            Y_ARGS_EXPOSE(T,Type);
            typedef NODE<T> NodeType;

            explicit SoloCache() noexcept : pool(sizeof(NodeType)) {}
            virtual ~SoloCache() noexcept  {}



            inline size_t stowage() const noexcept { return pool.inside(); }

            inline NodeType *produce(Type &args)
            {
                NodeType *node = pool.inside() ? static_cast<NodeType*>(pool.zquery()) : Object::zacquire<NodeType>();
                try {
                    return new (node) NodeType(args);
                }
                catch(...)
                {
                    pool.zstore(node);
                    throw;
                }
            }


            inline void destroy(NodeType *node) noexcept
            {
                assert(0!=node);
                pool.zstore( Destructed(node) );
            }

            Inventory pool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SoloCache);
        };

        template <typename T, template <typename> class NODE>
        class SoloProxy
        {
        public:
            typedef SoloCache<T,NODE> CacheType;

            explicit SoloProxy() noexcept : cache() {}
            explicit SoloProxy(const SoloProxy &) noexcept : cache() {}
            virtual ~SoloProxy() noexcept
            {
            }

            CacheType       * operator->() noexcept       { return &cache; }
            const CacheType * operator->() const noexcept { return &cache; }

        private:
            Y_DISABLE_ASSIGN(SoloProxy);
            CacheType cache;

        };

    }

    namespace Small
    {
        template <typename T, template <typename> class NODE>
        class CoopCache : public Object, public Counted, public SoloCache<T,NODE>
        {
        public:
            Y_ARGS_EXPOSE(T,Type);
            explicit CoopCache() noexcept {}
            virtual ~CoopCache() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CoopCache);
        };

        template <typename T, template <typename> class NODE>
        class CoopProxy
        {
        public:
            typedef CoopCache<T,NODE> CacheType;

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
    ListOf< Small::HeavyNode<int> > heavyList;
    ListOf< Small::LightNode<int> > lightList;
    int i = 2;

    {
        Small::BareProxy<int,Small::HeavyNode> heavyBareProxy;
        Small::BareProxy<int,Small::LightNode> lightBareProxy;

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
        Small::SoloProxy<int,Small::HeavyNode> heavySoloProxy;
        Small::SoloProxy<int,Small::LightNode> lightSoloProxy;

        for(size_t k=0;k<10;++k)
        {
            heavyList.pushTail( heavySoloProxy->produce(i) );
            lightList.pushTail( lightSoloProxy->produce(i) );
        }

        while(heavyList.size) heavySoloProxy->destroy( heavyList.popTail() );
        while(lightList.size) lightSoloProxy->destroy( lightList.popTail() );

        std::cerr << "stowage: " << heavySoloProxy->stowage() << " / " << lightSoloProxy->stowage() << std::endl;
    }
    
    //Small::CoopProxy<int,Small::HeavyNode>

    {
        Small::CoopProxy<int,Small::HeavyNode> heavyCoopProxy;
        Small::CoopProxy<int,Small::LightNode> lightCoopProxy;

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


