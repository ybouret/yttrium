//! \file

#ifndef Y_Field1D_Included
#define Y_Field1D_Included 1

#include "y/memory/allocator.hpp"
#include "y/container/operating.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{
    typedef unit_t Coord1D;

    class Field
    {
    public:
        const unsigned dims;

        virtual ~Field() noexcept {}
        explicit Field(const unsigned d) noexcept :
        dims(d)
        {}

        size_t getItems(unit_t * const width,
                        unit_t * const lower,
                        unit_t * const upper) const noexcept
        {
            size_t items = 1;
            for(unsigned d=0;d<dims;++d)
            {
                unit_t &lo = lower[d];
                unit_t &up = upper[d];
                if(up<lo) Swap(lo,up);
                items *= ( width[d] = (1+up)-lo );
            }
            return items;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Field);
    };

    template <typename T>
    class Field1D : public Field
    {
    public:
        static const unsigned DIMS = 1;
        Y_ARGS_DECL(T,Type);
        typedef Coord1D Coord;

        inline explicit Field1D(const Coord       lo,
                                const Coord       up,
                                Memory::Allocator &ma) :
        Field(DIMS),
        lower(lo),
        upper(up),
        width(lo),
        items(getItems( &Coerce(width), &Coerce(lower), &Coerce(upper) ) ),
        bytes(0),
        entry( ma.allocate<MutableType>(items,bytes) - lower ),
        alloc( &ma )
        {

        }

        inline virtual ~Field1D() noexcept
        {
        }

        const Coord lower;
        const Coord upper;
        const Coord width;

    private:
        size_t             items;
        size_t             bytes;
        MutableType       *entry;
        Memory::Allocator *alloc;

        Y_DISABLE_COPY_AND_ASSIGN(Field1D);
    };

}

#endif

