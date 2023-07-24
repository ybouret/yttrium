//! \file

#ifndef Y_Cxx_Series_Included
#define Y_Cxx_Series_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/workspace.hpp"
#include "y/container/writable.hpp"
#include "y/sequence/interface.hpp"
#include "y/container/recyclable.hpp"

namespace Yttrium
{


    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! Base class for CxxSeries
        //
        //______________________________________________________________________
        class CxxSeries
        {
        public:     static const char * const CallSign;  //!< "CxxSeries"
        protected:  explicit CxxSeries() noexcept;       //!< setup
        public:     virtual ~CxxSeries() noexcept;       //!< cleanup
        private: Y_DISABLE_COPY_AND_ASSIGN(CxxSeries);
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
    template <typename T, typename ALLOCATOR>
    class CxxSeries :
    public Memory::Wad<T,ALLOCATOR>,
    public Writable<T>,
    public Sequence<T>,
    public Recyclable,
    public Core::CxxSeries
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Memory::Wad<T,ALLOCATOR> WadType; //!< alias
        Y_ARGS_DECL(T,Type);                    //!< aliases
        typedef Memory::OutOfReach       MemOps;  //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup with default capacityy
        inline explicit CxxSeries(const size_t n) :
        WadType(n),
        Writable<T>(),
        Sequence<T>(),
        Core::CxxSeries(),
        entry( static_cast<MutableType *>(this->workspace)-1 ),
        count( 0 ),
        total( n )
        {
        }

        inline virtual ~CxxSeries() noexcept { free_(); }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual size_t size()           const noexcept { return count; } //!< fixed size
        inline virtual size_t capacity()       const noexcept { return total; } //!< fixed capacity
        inline virtual const char * callSign() const noexcept { return CallSign; }

        //! access
        inline virtual Type & operator[](const size_t i) noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }

        //! access, const
        inline virtual ConstType & operator[](const size_t i) const noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }


        inline virtual void popTail() noexcept
        {
            assert(count>0); trim_();
        }

        inline virtual void popHead() noexcept
        {
            assert(count>0);
            assert(0!=entry);
            MutableType *target = &entry[1];
            MemOps::Grab(MemOps::Naught(target),target+1,--Coerce(count)*sizeof(T));
        }

        inline virtual void pushTail(ParamType args)
        {
            assert(count<total);
            assert(0!=entry);
            new ( & entry[count+1] ) MutableType(args);
            ++Coerce(count);
        }

        inline virtual void pushHead(ParamType args)
        {
            assert(count<total);
            assert(0!=entry);
            const size_t dataSize = count * sizeof(T);
            MutableType *source   = &entry[1];
            MutableType *target   = source+1;
            MemOps::Move(target,source,dataSize);
            try {
                new ( MemOps::Zero(source,sizeof(T)) ) MutableType(args);
                ++Coerce(count);
            }
            catch(...)
            {
                MemOps::Grab(source,target,dataSize);
                throw;
            }
        }


        virtual void free() noexcept { free_(); }

        inline Type *      legacy()       noexcept { return entry; } //!< legacy [1:size()] C-style array
        inline ConstType * legacy() const noexcept { return entry; } //!< legacy [1:size()] C-style array


    protected:
        MutableType * const entry; //!< memory for [1..count]
        const size_t        count; //!< built objecct
        const size_t        total; //!< initial capacity

        inline void trim_() noexcept { assert(count>0); MemOps::Naught( &entry[ Coerce(count)-- ]); }
        inline void free_() noexcept { while(count>0) trim_(); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(CxxSeries);

    };



}

#endif

