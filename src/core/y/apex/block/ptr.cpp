
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
            host->withhold();
        }

        BlockPtr:: ~BlockPtr() noexcept
        {
            assert(0!=host);
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

    }

}


