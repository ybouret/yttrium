
//! \file

#ifndef Y_Memory_Allocator_Dyadic_Included
#define Y_Memory_Allocator_Dyadic_Included 1

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
        //! Allocator of Dyadic chunks based on internal Corpus
        //
        //
        //______________________________________________________________________
        class Dyadic : public Singleton<Dyadic,NucleusSingleton>, public Allocator
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign; //!< "Memory::Dyadic"
            static const AtExit::Longevity LifeTime; //!< Pooled::LifeTime - 1;

            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            virtual void *       acquire(size_t & count, const size_t blockSize);      //!< count will be a power of two
            virtual void         release(void * & entry, size_t &     count) noexcept; //!< release previously acquired
            virtual const char * variety()                             const noexcept; //!< CallSign

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dyadic);
            explicit Dyadic();
            virtual ~Dyadic() noexcept;
            friend SingletonType;

        };
    }

}

#endif

