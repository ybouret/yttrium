
#ifndef Y_Container_CxxArray_Included
#define Y_Container_CxxArray_Included 1

#include "y/container/writable.hpp"
#include "y/container/operating.hpp"
#include "y/memory/wad.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Local Array of C++ objects
    //
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR>
    class CxxArray :
    public Memory::Wad<T,ALLOCATOR>,
    public Operating<T>,
    public Writable<T>
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef Memory::Wad<T,ALLOCATOR> WadType; //!< alias
        typedef Operating<T>             OpsType; //!< alias
        Y_ARGS_EXPOSE(T);                         //!< aliases

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup with default [1:n] objects
        inline explicit CxxArray(const size_t n) :
        WadType(n), OpsType(this->workspace,n), Writable<T>(),
        entry( static_cast<MutableType *>(this->workspace)-1 ),
        count( n )
        {
        }

        inline virtual ~CxxArray() noexcept {}

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual size_t size()     const noexcept { return count; } //!< fixed size
        inline virtual size_t capacity() const noexcept { return count; } //!< fixed capacity

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
        MutableType * const entry; //!< memory for [1..count]
        const size_t        count; //!< built objecct
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(CxxArray);

    };


}

#endif

