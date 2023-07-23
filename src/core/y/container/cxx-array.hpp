
#ifndef Y_Container_CxxArray_Included
#define Y_Container_CxxArray_Included 1

#include "y/container/writable.hpp"
#include "y/container/operating.hpp"
#include "y/memory/wad.hpp"

namespace Yttrium
{

    template <typename T, typename ALLOCATOR>
    class CxxArray :
    public Memory::Wad<T,ALLOCATOR>,
    public Operating<T>,
    public Writable<T>
    {
    public:
        typedef Memory::Wad<T,ALLOCATOR> WadType;
        typedef Operating<T>             OpsType;

        Y_ARGS_EXPOSE(T);

        inline explicit CxxArray(const size_t n) :
        WadType(n), OpsType(this->workspace,n), Writable<T>(),
        entry( static_cast<MutableType *>(this->workspace)-1 ),
        count( n )
        {
        }

        inline virtual ~CxxArray() noexcept {}

        inline virtual size_t size()     const noexcept { return count; }
        inline virtual size_t capacity() const noexcept { return count; }

        inline virtual Type & operator[](const size_t i) noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }

        inline virtual ConstType & operator[](const size_t i) const noexcept {
            assert(i>0); assert(i<=size()); return entry[i];
        }

        inline Type *      legacy()       noexcept { return entry; }
        inline ConstType * legacy() const noexcept { return entry; }

        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(CxxArray);
        MutableType * const entry;
        const size_t        count;
    };


}

#endif

