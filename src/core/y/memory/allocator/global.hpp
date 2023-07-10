//! \file

#ifndef Y_Memory_Allocator_Global_Included
#define Y_Memory_Allocator_Global_Included 1

#include "y/singleton.hpp"
#include "y/memory/allocator.hpp"

namespace Yttrium
{

    namespace Memory
    {

        class Global : public Singleton<Global,NucleusSingleton>, public Allocator
        {
        public:
            static const char * const      CallSign; //!< "Memory::Global"
            static const AtExit::Longevity LifeTime; //!< AtExit::MaximumLongevity - 1;
            
            virtual void *       acquire(size_t & count, const size_t blockSize);
            virtual void         release(void * & entry, size_t &     count) noexcept;
            virtual const char * variety()                             const noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Global);
            explicit Global();
            virtual ~Global() noexcept;
            friend SingletonType;
            
        };


    }

}

#endif

