//! \file

#ifndef Y_Container_CxxArray_Included
#define Y_Container_CxxArray_Included 1

#include "y/container/writable.hpp"
#include "y/container/operating.hpp"
#include "y/container/iterator/writable-contiguous.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/wad.hpp"

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
        };

    }



    //__________________________________________________________________________
    //
    //
    //! CxxArray constructor prolog
    //
    //__________________________________________________________________________
#define Y_CxxArray_Prolog(COUNT) \
Identifiable(), Collection(),    \
Writable<T>(),                   \
WritableContiguous<T>(),         \
Core::CxxArray(),                \
WadType(COUNT)

    //__________________________________________________________________________
    //
    //
    //! CxxArray constructor epilog
    //
    //__________________________________________________________________________
#define Y_CxxArray_Epilog(COUNT) \
cdata( this->lead() ),           \
entry( this->warp() ),           \
count( COUNT )

    //__________________________________________________________________________
    //
    //
    //
    //! Local Array of C++ objects
    //
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = Memory::Pooled>
    class CxxArray :
    public Writable<T>,
    public WritableContiguous<T>,
    public Core::CxxArray,
    public Memory::Wad<T,ALLOCATOR>,
    public Operating<T>
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
        Y_ARGS_DECL(T,Type);                                //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup with DEFAULT  objects[1:n]
        inline explicit CxxArray(const size_t n) :
        Y_CxxArray_Prolog(n),
        OpsType(this->workspace,n),
        Y_CxxArray_Epilog(n)
        {
        }

        //! setup with same value objects[1:n]
        inline explicit CxxArray(const size_t n, ParamType value) :
        Y_CxxArray_Prolog(n),
        OpsType(this->workspace,n,value),
        Y_CxxArray_Epilog(n)
        {

        }

        //! setup with 1-arg constructor [1:n] objets
        template <typename U>
        inline explicit CxxArray(const size_t n, const CopyOf_ &, U &args ) :
        Y_CxxArray_Prolog(n),
        OpsType(this->workspace,n,args),
        Y_CxxArray_Epilog(n)
        {

        }

        //! setup by copy of sequence-like source
        template <typename SOURCE>
        inline explicit CxxArray(const CopyOf_ &copying, SOURCE &src) :
        Y_CxxArray_Prolog(src.size()),
        OpsType(this->workspace,copying,src),
        Y_CxxArray_Epilog(src.size())
        {
        }

        //! default copy constructor
        inline explicit CxxArray(const CxxArray &src) :
        Y_CxxArray_Prolog(src.size()),
        OpsType(this->workspace,src),
        Y_CxxArray_Epilog(src.size())
        {
        }

        //! cleanup
        inline virtual ~CxxArray() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual size_t       size()     const noexcept { return count; }    //!< fixed size
        inline virtual size_t       capacity() const noexcept { return count; }    //!< fixed capacity
        inline virtual const char * callSign() const noexcept { return CallSign; } //!< CallSign

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
        const size_t        count; //!< built object(s)
        
    private:
        Y_DISABLE_ASSIGN(CxxArray);
        virtual ConstType *getBaseForward() const noexcept { return cdata; }
        virtual ConstType *getLastForward() const noexcept { return cdata+count; }

        virtual ConstType *getBaseReverse() const noexcept { return entry+count; }
        virtual ConstType *getLastReverse() const noexcept { return entry;       }
        
    };


}

#endif

