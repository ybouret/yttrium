//! \file

#ifndef Y_Memory_ZCache_Included
#define Y_Memory_ZCache_Included 1

#include "y/lockable.hpp"
#include "y/data/pool.hpp"
#include "y/type/releasable.hpp"

namespace Yttrium
{
    namespace Memory
    {

        class Arena;

        //______________________________________________________________________
        //
        //
        //! alias to internally handled data blocks
        //
        //______________________________________________________________________
        struct ZombieNode
        {
            ZombieNode *next; //!< for pool/list
            ZombieNode *prev; //!< for list
        };


        //______________________________________________________________________
        //
        //
        //! base class for ZombieCache
        //
        //______________________________________________________________________
        typedef PoolOf<ZombieNode> ZombiePool;

        //______________________________________________________________________
        //
        //
        //
        //! User's Level cache of data blocks
        //
        //
        //______________________________________________________________________
        class ZombieCache : public ZombiePool, public Releasable
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup with (minimal) blockSize and initial capacity
            explicit ZombieCache(const size_t userBlockSize,
                                 const size_t startCapacity);

        public:
            //! cleanup
            virtual ~ZombieCache() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void        reserve(size_t n);  //!< populate cache with more blocks
        protected:
            void       *acquireBlock();    //!< acquire new/query a block

        public:
            virtual void release()        noexcept; //!< Releasable interface
            void         zrelease(void *) noexcept; //!< release a previously acquired

            //__________________________________________________________________
            //
            //
            // members
            //
            //__________________________________________________________________
            const size_t blockSize; //!< adjusted blockSize
            Lockable    &giantLock; //!< to access coreArena
            Arena       &coreArena; //!< Quark's arena

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ZombieCache);
            void empty() noexcept;
        };

    }


    //__________________________________________________________________________
    //
    //
    //
    //! User's Level cache for a given data type
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class ZombieCache : public Memory::ZombieCache
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! initialize
        inline explicit ZombieCache(const size_t startCapacity) : Memory::ZombieCache(sizeof(T), startCapacity) {}

        //! cleanup
        inline virtual ~ZombieCache() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! acquire a zombie object
        inline T *  zacquire() { return static_cast<T*>(acquireBlock()); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ZombieCache);
    };
    
}

#endif

