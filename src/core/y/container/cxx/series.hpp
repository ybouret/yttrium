//! \file

#ifndef Y_Cxx_Series_Included
#define Y_Cxx_Series_Included 1

#include "y/memory/wad.hpp"
#include "y/memory/solitary/workspace.hpp"
#include "y/container/writable.hpp"
#include "y/sequence/interface.hpp"
#include "y/container/recyclable.hpp"
#include "y/container/iterator/writable-contiguous.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/type/copy.hpp"

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
        public:    static const char * const CallSign;  //!< "CxxSeries"
        protected: explicit CxxSeries() noexcept;       //!< setup
        public:    virtual ~CxxSeries() noexcept;       //!< cleanup
        private:   Y_DISABLE_COPY_AND_ASSIGN(CxxSeries);
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
    template <typename T, typename ALLOCATOR = Memory::Pooled >
    class CxxSeries :
    public Memory::Wad<T,ALLOCATOR>,
    public Writable<T>,
    public Sequence<T>,
    public Recyclable,
    public WritableContiguous<T>,
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
        typedef Memory::OutOfReach       MemOps;  //!< alias
        Y_ARGS_DECL(T,Type);                      //!< aliases

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
        cdata( this->lead() ),
        entry( this->warp() ),
        count( 0 ),
        total( n )
        {
        }

        //! duplicate
        template <typename SOURCE>
        inline CxxSeries(const CopyOf_ &, SOURCE &other) :
        WadType( other.size() ),
        Writable<T>(),
        Sequence<T>(),
        Core::CxxSeries(),
        cdata( this->lead() ),
        entry( this->warp() ),
        count( 0 ),
        total( other.size() )
        {
            try
            {
                assert( capacity() >= other.size() );
                const size_t n = other.size();
                for(size_t i=1;i<=n;++i)
                   (void) grow( other[i] );
            }
            catch(...) { free_(); throw; }
        }

        inline virtual ~CxxSeries() noexcept { free_(); }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual size_t       size()     const noexcept { return count;    } //!< fixed size
        inline virtual size_t       capacity() const noexcept { return total;    } //!< fixed capacity
        inline virtual const char * callSign() const noexcept { return CallSign; }

        //! access
        inline virtual Type & operator[](const size_t i) noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }

        //! access, const
        inline virtual ConstType & operator[](const size_t i) const noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }


        //! pop tail
        inline virtual void popTail() noexcept
        {
            assert(count>0); trim_();
        }

        //! pop head
        inline virtual void popHead() noexcept
        {
            assert(count>0);
            assert(0!=entry);
            MutableType * const target = &entry[1];
            MemOps::Grab(MemOps::Naught(target),target+1,--Coerce(count)*sizeof(T));
        }

        //! push args at tail
        inline virtual void pushTail(ParamType args) {
            (void) grow(args);
        }

        //! push args at head
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
        
        //! free content
        virtual void free() noexcept { free_(); }
        
        //! grow with one arg constiructor
        template <typename U>
        inline Type & grow(U &args) {
            assert(count<total);
            assert(0!=entry);
            Type * const addr =new ( & entry[count+1] ) MutableType(args);
            ++Coerce(count);
            return *addr;
        }

        //! grow with two-args constructor
        template <typename U, typename V>
        inline Type & grow(U &u, V &v) {
            assert(count<total);
            assert(0!=entry);
            Type * const addr = new ( & entry[count+1] ) MutableType(u,v);
            ++Coerce(count);
            return *addr;
        }

        //! grow with three args constructor
        template <typename U, typename V, typename W>
        inline Type & grow(U &u, V &v, W &w) {
            assert(count<total);
            assert(0!=entry);
            Type * const addr = new ( & entry[count+1] ) MutableType(u,v,w);
            ++Coerce(count);
            return *addr;
        }

        //! remove item[i]
        inline void remove(const size_t i) noexcept
        {
            assert(i>=1);
            assert(i<=count);
            MutableType * const target = &entry[i];
            Memory::OutOfReach::Move( Destructed(target),target+1,(count-i) * sizeof(Type));
            Memory::OutOfReach::Zero( &entry[count], sizeof(Type) );
            --Coerce(count);
        }


    protected:
        MutableType * const cdata; //!< memory for [0..count-1]
        MutableType * const entry; //!< memory for [1..count]
        const size_t        count; //!< built object
        const size_t        total; //!< initial capacity
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(CxxSeries);
        inline void trim_() noexcept { assert(count>0); MemOps::Naught( &entry[ Coerce(count)-- ]); }
        inline void free_() noexcept { while(count>0) trim_(); }





        virtual ConstType *getBaseForward() const noexcept { return cdata; }
        virtual ConstType *getLastForward() const noexcept { return cdata+count; }

        virtual ConstType *getBaseReverse() const noexcept { return entry+count; }
        virtual ConstType *getLastReverse() const noexcept { return entry;       }

        virtual ConstType & getHead() const noexcept { assert(count>0); return *cdata; }
        virtual ConstType & getTail() const noexcept { assert(count>0); return entry[count]; }


    };



}

#endif

