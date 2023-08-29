//! \file
//! 
#ifndef Y_Container_CxxArray_Included
#define Y_Container_CxxArray_Included 1

#include "y/container/writable.hpp"
#include "y/container/operating.hpp"
#include "y/memory/wad.hpp"
#include "y/container/iterator/writable-contiguous.hpp"


namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Base class for CxxArrays
        //
        //______________________________________________________________________
        class CxxArray
        {
        public:     static const char * const CallSign; //!< "CxxArray"
        protected:  explicit CxxArray() noexcept;       //!< setup
        public:     virtual ~CxxArray() noexcept;       //!< cleanup
        private: Y_DISABLE_COPY_AND_ASSIGN(CxxArray);
        public:

            template <bool> struct  GetSize;

            template <>  struct GetSize<true> {
                static inline size_t From(const size_t n, const Memory::Crux::Wad &) noexcept
                {
                    return n;
                }
            };

            template <>  struct GetSize<false> {
                static inline size_t From(const size_t, const Memory::Crux::Wad &w) noexcept
                {
                    return w.maxBlocks;
                }
            };

        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Local Array of C++ objects
    //
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR, bool UseAssigned = true >
    class CxxArray :
    public Memory::Wad<T,ALLOCATOR>,
    public Operating<T>,
    public Writable<T>,
    public WritableContiguous<T>,
    public Core::CxxArray
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Memory::Wad<T,ALLOCATOR>             WadType; //!< alias
        typedef Operating<T>                         OpsType; //!< alias
        typedef Core::CxxArray::GetSize<UseAssigned> GetSize; //!< decide size
        Y_ARGS_EXPOSE(T,Type);                                //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup with default [1:n/max] objects
        inline explicit CxxArray(const size_t n) :
        WadType(n),
        OpsType(this->workspace,GetSize::From(n,*this)),
        Writable<T>(),
        cdata( static_cast<MutableType *>(this->workspace) ),
        entry( cdata-1 ),
        count( GetSize::From(n,*this) )
        {
        }

        inline virtual ~CxxArray() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual size_t       size()           const noexcept { return count; } //!< fixed size
        inline virtual size_t       capacity() const noexcept { return count; } //!< fixed capacity
        inline virtual const char * callSign() const noexcept { return CallSign; }

        //! access
        inline virtual Type & operator[](const size_t i) noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }

        //! access, const
        inline virtual ConstType & operator[](const size_t i) const noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }

        inline Type *      legacy()       noexcept { return entry; } //!< legacy [1:size()] C-style array
        inline ConstType * legacy() const noexcept { return entry; } //!< legacy [1:size()] C-style array


    protected:
        MutableType * const cdata; //!< memory or [0..count-1]
        MutableType * const entry; //!< memory for [1..count]
        const size_t        count; //!< built objecct
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(CxxArray);
        virtual ConstType *getBaseForward() const noexcept { return cdata; }
        virtual ConstType *getLastForward() const noexcept { return cdata+count; }

        virtual ConstType *getBaseReverse() const noexcept { return entry+count; }
        virtual ConstType *getLastReverse() const noexcept { return entry;       }
        
    };


}

#endif

