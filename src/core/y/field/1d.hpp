//! \file

#ifndef Y_Field1D_Included
#define Y_Field1D_Included 1

#include "y/field/layout.hpp"
#include "y/memory/allocator.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{
    namespace Field
    {
        typedef unit_t          Coord1D;
        typedef Layout<Coord1D> Layout1D;

        template <typename T>
        class In1D : public Layout1D
        {
        public:
            Y_ARGS_DECL(T,Type);
            typedef Layout1D               LayoutType;

            inline In1D(LayoutType         layout,
                        Memory::Allocator &mgr) :
            LayoutType(layout),
            count(items),
            bytes(0),
            entry( mgr.allocate<MutableType>(count,bytes) - lower ),
            alloc( &mgr ),
            built(0)
            {
                setup();
            }

            inline virtual ~In1D() noexcept
            {
                clear();
            }

            inline MutableType & operator[](const Coord1D q) noexcept
            {
                assert(q>=lower); assert(q<=upper);
                return entry[q];
            }

            inline ConstType & operator[](const Coord1D q) const noexcept
            {
                assert(q>=lower); assert(q<=upper);
                return entry[q];
            }


        private:
            size_t              count;
            size_t              bytes;
            MutableType *       entry;
            Memory::Allocator * alloc;
            size_t              built;

            Y_DISABLE_COPY_AND_ASSIGN(In1D);

            inline void clear() noexcept
            {
                if(0!=alloc)
                {
                    entry += lower;
                    while(built>0)
                        Memory::OutOfReach::Naught( &entry[--built] );
                    alloc->withdraw(entry,count,bytes);
                    alloc = 0;
                }

            }

            inline void setup() {
                try {
                    MutableType *addr = entry+lower;
                    while(built<items) {
                        new (addr+built) MutableType();
                        ++built;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
            }
        };

    }




#if 0
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
        built(0),
        items(getItems( &Coerce(width), &Coerce(lower), &Coerce(upper) ) ),
        bytes(0),
        entry( ma.allocate<MutableType>(items,bytes) - lower ),
        alloc( &ma )
        {
            setup();
        }

        inline virtual ~Field1D() noexcept { release(); }

        const Coord lower;
        const Coord upper;
        const Coord width;

    private:
        size_t                   built;
        size_t                   items;
        size_t                   bytes;
        MutableType             *entry;
        Memory::Allocator       *alloc;

        Y_DISABLE_COPY_AND_ASSIGN(Field1D);
        inline void setup() {
            assert(0==built);
            try        { for(unit_t i=lower;i<=upper;++i) newAt(i); }
            catch(...) { release(); throw; }
        }

        inline void release() noexcept
        {
            if(0!=alloc)
            {
                entry             += lower;
                while(built>0)
                    Memory::OutOfReach::Naught( &entry[--built]);
                alloc->withdraw(entry, items, bytes);
                alloc = 0;
            }
            clear();
        }

        inline void newAt(const unit_t i) { new (entry+i) MutableType(); ++built; }
      
        inline void clear() noexcept
        {
            Y_STATIC_ZVAR(Coerce(lower));
            Y_STATIC_ZVAR(Coerce(upper));
            Y_STATIC_ZVAR(Coerce(width));
        }

    };
#endif
    
}

#endif

