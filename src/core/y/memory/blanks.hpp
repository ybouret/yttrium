//! \file

#ifndef Y_Memory_Blanks_Included
#define Y_Memory_Blanks_Included 1

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
        struct BlankNode
        {
            BlankNode *next; //!< for pool/list
            BlankNode *prev; //!< for list
            typedef PoolOf<BlankNode> Pool;
        };



        //______________________________________________________________________
        //
        //
        //
        //! User's Level cache of data blocks
        //
        //
        //______________________________________________________________________
        class Blanks : public BlankNode::Pool, public Releasable
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup with (minimal) blockSize and initial capacity
            explicit Blanks(const size_t userBlockSize,
                            const size_t startCapacity);

        public:
            //! cleanup
            virtual ~Blanks() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void        reserve(size_t n);  //!< populate cache with more blocks
        protected:
            void       *acquireBlock();    //!< [locked acquire new|query a block]

        public:
            virtual void release()        noexcept; //!< Releasable interface
            void         zrelease(void *) noexcept; //!< release a previously acquired

            void         criticalCheck(const size_t blockSize,
                                       const char  *context) const noexcept;

            void         displayInfo(const size_t indent) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t allocated;
        private:
            Lockable    &giantLock; //!< to access coreArena
            Arena       &coreArena; //!< Quark's arena

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blanks);
            void empty() noexcept; //!< return to locked arena
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
    class Blanks : public Memory::Blanks
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! initialize
        inline explicit Blanks(const size_t startCapacity) : Memory::Blanks(sizeof(T), startCapacity) {}

        //! cleanup
        inline virtual ~Blanks() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! acquire a zombie object
        inline T *  zacquire() { return static_cast<T*>(acquireBlock()); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Blanks);
    };
    
}

#endif

