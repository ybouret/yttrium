//! \file

#ifndef Y_Memory_Blanks_Included
#define Y_Memory_Blanks_Included 1

#include "y/type/cache.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! User's Level cache of data blocks
        //
        //
        //______________________________________________________________________
        class Blanks : public Cache
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Memory::Blanks"
            
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
        protected:
            void       *fetchBlank();                        //!< (locked) [acquire new|query a] blank block
        public:
            virtual void release()                 noexcept; //!< [Releasable] empty available memory
            virtual void gc(const size_t maxCount) noexcept; //!< [Cache] keep no more than maxCount blocks
            void         storeBlank(void *)        noexcept; //!< store a previously acquired
            void         eraseBlank(void *)        noexcept; //!< (locked) directly sent to arena
            size_t       blockSize()         const noexcept; //!< internal arena blockSize
            size_t       allocated()         const noexcept; //!< bookkept allocated blocks
            size_t       available()         const noexcept; //!< available blocks
            void         displayInfo(const size_t)    const; //!< display statistics
            void         reserve(const size_t);              //!< populate cache with more blocks
            void         swapWith(Blanks &other)   noexcept; //!< exchange content
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blanks);
            class Code;
            Code *code;

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
        inline T *  queryBlank() { return static_cast<T*>(fetchBlank()); }
        
        //! store a built object, keeping memory in cache
        inline void storeBuilt(T *obj) noexcept { assert(0!=obj); storeBlank( Destructed(obj) ); }

        //! erase a built object, returning memory to system
        inline void eraseBuilt(T *obj) noexcept { assert(0!=obj); eraseBlank( Destructed(obj) ); }
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Blanks);
    };
    
}

#endif

