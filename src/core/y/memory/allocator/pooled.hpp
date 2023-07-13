
//! \file

#ifndef Y_Memory_Allocator_Pooled_Included
#define Y_Memory_Allocator_Pooled_Included 1

#include "y/singleton.hpp"
#include "y/memory/allocator.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! Allocator based on Quark.straps
        //
        //
        //______________________________________________________________________
        class Pooled : public Singleton<Pooled,NucleusSingleton>, public Allocator
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                //!< "Memory::Pooled"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 2; //!< longevity

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void *       acquire(size_t & count, const size_t blockSize);      //!< never empty
            virtual void         release(void * & entry, size_t &     count) noexcept; //!< release a previously acquired
            virtual const char * variety()                             const noexcept; //!< CallSign

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pooled);
            explicit Pooled();
            virtual ~Pooled() noexcept;
            friend class Singleton<Pooled,NucleusSingleton>;

        };


    }

}

#endif
