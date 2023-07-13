//! \file

#ifndef Y_Memory_Allocator_Global_Included
#define Y_Memory_Allocator_Global_Included 1

#include "y/singleton.hpp"
#include "y/memory/allocator.hpp"
#include "y/locking/nucleus.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Allocator of system wide memory
        //
        //
        //______________________________________________________________________
        class Global : public Singleton<Global,Locking::Nucleus>, public Allocator
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                //!< "Memory::Global"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 1; //!< longevity

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual void *       acquire(size_t & count, const size_t blockSize);      //!< any count of any blockSize
            virtual void         release(void * & entry, size_t &     count) noexcept; //!< release a previously acquired
            virtual const char * variety()                             const noexcept; //!< CallSign
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Global);
            explicit Global();
            virtual ~Global() noexcept;
            friend class Singleton<Global,Locking::Nucleus>;
            
        };


    }

}

#endif

