
//! \file

#ifndef Yttrium_Memory_Allocator_Included
#define Yttrium_Memory_Allocator_Included 1


#include "y/config/starting.hpp"

namespace Yttrium
{
    
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Allocator interface
        //
        //
        //_______________________________________________________________________
        class Allocator
        {
        public:
            //__________________________________________________________________
            //
            //
            // virtual interface
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! calloc style acquire
            /**
             \param count      number of blockSize, set to count*blockSize upon success, 0 upon failure
             \param blockSize  blockSize to allocate
             \return an area of count = count*blockSize bytes
             */
            //__________________________________________________________________
            virtual void *        acquire(size_t & count, const size_t blockSize)      = 0;

            //__________________________________________________________________
            //
            //! release previously acquire memory
            /**
             entry and count are set to 0
             \param entry  previously acquired block
             \param count  previously acquired bytes
             */
            //__________________________________________________________________
            virtual void          release(void * & entry, size_t &     count) noexcept = 0;

            //__________________________________________________________________
            //
            //! return a human readable name for the allocator
            //__________________________________________________________________
            virtual const char *  variety()                             const noexcept = 0;

            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Allocator() noexcept; //!< cleanup
        protected:
            explicit Allocator() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Allocator);
        };
    }
}

#endif

