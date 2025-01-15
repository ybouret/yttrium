
#include "y/apex/block/mtx.hpp"
#include "y/apex/block/factory.hpp"

namespace Yttrium
{
    namespace Apex
    {
        MutexPtr:: MutexPtr(Mutex * const mtx) noexcept :
        Core::Ptr(),
        host(mtx)
        {
            assert(0!=mtx);
            assert( Factory::Exists() );
            host->withhold();
        }

        MutexPtr:: ~MutexPtr() noexcept
        {
            assert(0!=host);
            static Factory &factory = Factory::Location();
            if(host->liberate())
            {
                factory.store(host);
            }
            Coerce(host) = 0;
        }


        MutexPtr:: MutexPtr(const MutexPtr &_) noexcept :
        Core::Ptr(),
        host(_.host)
        {
            assert(0!=host);
            host->withhold();
        }

        MutexType * MutexPtr:: operator->() noexcept {
            assert(0!=host);
            return host;
        }

        MutexType & MutexPtr:: operator*()  noexcept
        {
            assert(0!=host);
            return *host;
        }


        const MutexType * MutexPtr:: operator->() const noexcept {
            assert(0!=host);
            return host;
        }

        const MutexType & MutexPtr:: operator*() const noexcept
        {
            assert(0!=host);
            return *host;
        }
    }

}

