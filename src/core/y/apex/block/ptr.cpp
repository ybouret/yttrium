
#include "y/apex/block/ptr.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {
        BlockPtr:: BlockPtr(Block * const block) noexcept :
        Core::Ptr(),
        host(block)
        {
            assert(0!=block);
            assert( Factory::Exists() );
            host->withhold();
        }

#if 0
        static inline
        Block * queryBlock(const size_t bytes)
        {
            static Factory &factory = Factory::Instance();
            return factory.queryBytes(bytes);
        }

        BlockPtr:: BlockPtr(const size_t bytes,
                            const AsCapacity_ &) :
        Core::Ptr(),
        host( queryBlock(bytes) )
        {
            assert(host->range>=bytes);
            host->withhold();
        }
#endif

        BlockPtr:: ~BlockPtr() noexcept
        {
            assert(0!=host);
            assert(Factory::Exists());
            static Factory &factory = Factory::Location();
            if(host->liberate())
            {
                factory.store(host);
            }
            Coerce(host) = 0;
        }


        BlockPtr:: BlockPtr(const BlockPtr &_) noexcept :
        Core::Ptr(),
        host(_.host)
        {
            assert(0!=host);
            host->withhold();
        }

        Block * BlockPtr:: operator->() noexcept {
            assert(0!=host);
            return host;
        }

        Block & BlockPtr:: operator*()  noexcept
        {
            assert(0!=host);
            return *host;
        }


        const Block * BlockPtr:: operator->() const noexcept {
            assert(0!=host);
            return host;
        }

        const Block & BlockPtr:: operator*() const noexcept
        {
            assert(0!=host);
            return *host;
        }

        void BlockPtr:: swp(BlockPtr &other) noexcept
        {
            CoerceSwap(host,other.host);
        }


    }

}


