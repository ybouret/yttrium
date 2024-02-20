//! \file

#ifndef Y_Memory_Wad_Included
#define Y_Memory_Wad_Included 1

#include "y/memory/allocator.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{
    namespace Memory
    {

        namespace Crux
        {
            //__________________________________________________________________
            //
            //
            //
            //! internal memory operations for typed allocations
            //
            //
            //__________________________________________________________________
            class Wad
            {
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! internal allocation
                /**
                 \param allocator user's allocator
                 \param numBlocks minimial blocks to acquire
                 \param blockSize > 0, bytes for each block
                 */
                //______________________________________________________________
            protected: explicit Wad(Allocator   &allocator,
                                    const size_t numBlocks,
                                    const size_t blockSize) noexcept;
            public:    virtual ~Wad()                       noexcept; //!< check clean

            protected: void *       workspace; //!< flat memory
            public:    const size_t capacity;  //!< acquired blocks
            public:    const size_t numBytes;  //!< acquired bytes

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
            protected:
                void         returnTo(Allocator &allocator)               noexcept; //!< return memory and cleanup

                //! transform [0..size-1] in [1..size]
                template <typename T> inline T *Cxx(T * const addr) noexcept
                {
                    return static_cast<T*>( WalkDown(addr,sizeof(T)) );
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Wad);
                static void *WalkDown(void *blockAddr, const size_t blockSize) noexcept; //!< &blockAddr[-blockSize]

            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Local allocation of memory blocks
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class Wad : public Crux::Wad
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! alias to produce mutable lead type
            typedef typename TypeTraits<T>::MutableType LeadType;

            //! acquire at least numbBlocks
            inline explicit Wad(const size_t numBlocks) :
            Crux::Wad(GetAllocatorInstance(),numBlocks,sizeof(T))
            {
            }

            //! release memory
            inline virtual ~Wad() noexcept {
                assert(ALLOCATOR::Exists());
                static Allocator &_ = ALLOCATOR::Location();
                returnTo(_);
            }

        protected:
            //! cast workspace to mutable pointed type [0..size-1]
            inline LeadType *lead() noexcept { return static_cast<LeadType *>(workspace); }

            //! cast workspace to a mutable pointe type [1..size]
            inline LeadType *warp() noexcept { return Cxx(lead()); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wad);
            static inline Allocator &GetAllocatorInstance() {
                static Allocator &_ = ALLOCATOR::Instance();
                return _;
            }


        };

    }
}

#endif

