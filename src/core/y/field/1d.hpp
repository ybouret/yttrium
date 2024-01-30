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
        class Builder
        {
        public:
            explicit Builder(T           *blockAddr,
                             const size_t numBlocks) :
            block(blockAddr),
            built(0)
            {
                assert(Good(blockAddr,numBlocks));
                try {
                    while(built<numBlocks) {
                        new (block+built) T();
                        ++built;
                    }
                }
                catch(...) { clearBlocks(); throw; }
            }

        protected:
            T     *block;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Builder);
            size_t built;
            inline void clearBlocks() noexcept
            {
                while(built>0) Memory::OutOfReach::Naught( &block[--built] );
                block = 0;
            }
        };

        


        template <typename T>
        class In1D : public Layout1D
        {
        public:
            Y_ARGS_DECL(T,Type);
            typedef Layout1D               LayoutType;

            inline In1D(LayoutType         layout,
                        Memory::Allocator &memmgr) :
            LayoutType(layout),
            count(items),
            bytes(0),
            entry(  memmgr.allocate<MutableType>(count,bytes) - lower ),
            alloc( &memmgr ),
            built(0)
            {
                setup();
            }

            inline virtual ~In1D() noexcept { clear(); }

            inline In1D(const LayoutType &layout,
                        MutableType      *aliens) noexcept :
            LayoutType(layout),
            count(0),
            bytes(0),
            entry(aliens-lower),
            alloc(0),
            built(0)
            {
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

            inline std::ostream & display(std::ostream &os) const
            {
                return Core::Display(os, entry+lower, width);
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





}

#endif

