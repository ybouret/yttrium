#include "y/data/small/list-node.hpp"
#include "y/data/small/repo-node.hpp"
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
        class BareCache
        {
        public:
            Y_ARGS_EXPOSE(T,Type);
            typedef NODE<T> NodeType;

            explicit BareCache() noexcept {}
            virtual ~BareCache() noexcept {}


            inline size_t stowage() const noexcept { return 0; }

            inline NodeType *acquire(Type &args)
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

            inline void release(NodeType *node) noexcept
            {
                Object::zrelease(Destructed(node));
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(BareCache);
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

            inline NodeType *acquire(Type &args)
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

            inline void release(NodeType *node) noexcept
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

    Small::BareProxy<int,Small::ListNode> ilBareProxy;
    Small::BareProxy<int,Small::RepoNode> irBareProxy;

    ListOf< Small::ListNode<int> > il;
    ListOf< Small::RepoNode<int> > ir;

    int i = 2;

    for(size_t k=0;k<10;++k)
    {
        il.pushTail( ilBareProxy->acquire(i) );
        ir.pushTail( irBareProxy->acquire(i) );
    }

    while(il.size) ilBareProxy->release( il.popTail() );
    while(ir.size) irBareProxy->release( ir.popTail() );


    std::cerr << "stowage: " << ilBareProxy->stowage() << " / " << irBareProxy->stowage() << std::endl;

    Small::SoloProxy<int,Small::ListNode> ilSoloProxy;
    Small::SoloProxy<int,Small::RepoNode> irSoloProxy;

    for(size_t k=0;k<10;++k)
    {
        il.pushTail( ilSoloProxy->acquire(i) );
        ir.pushTail( irSoloProxy->acquire(i) );
    }

    while(il.size) ilSoloProxy->release( il.popTail() );
    while(ir.size) irSoloProxy->release( ir.popTail() );

    std::cerr << "stowage: " << ilSoloProxy->stowage() << " / " << irSoloProxy->stowage() << std::endl;

    

}
Y_UDONE()


