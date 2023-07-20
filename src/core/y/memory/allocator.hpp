
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
            //! specialized allocator
            /**
             \param items positive of null items to get
             \param bytes output allocated flat bytes
             */
            //
            //__________________________________________________________________
            template <typename T> inline
            T * allocate(size_t &items,
                         size_t &bytes)
            {
                assert(0==bytes);
                size_t count = items <= 0 ? 1 : items;
                try {
                    void *wksp = acquire(count,sizeof(T));
                    assert(0!=wksp);
                    assert(count>0);
                    bytes = count;
                    items = count/sizeof(T);
                    return static_cast<T*>(wksp);
                }
                catch(...)
                {
                    assert(0==bytes);
                    items = 0;
                    throw;
                }
            }



            //__________________________________________________________________
            //
            //
            //! specialized deallocator
            /**
             \param entry previously allocated
             \param items positive of null items
             \param bytes allocated flat bytes
             */
            //
            //__________________________________________________________________
            template <typename T> inline
            void withdraw(T * &entry, size_t &items, size_t &bytes) noexcept
            {
                assert(NULL!=entry);
                assert(items>0);
                assert(bytes>=items*sizeof(T));
                release(*(void**)&entry,bytes);
                assert(0==entry);
                assert(0==bytes);
                items = 0;
            }


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

