//! \file

#ifndef Y_Field1D_Included
#define Y_Field1D_Included 1

#include "y/field/layout.hpp"
#include "y/field/memory/builder.hpp"
#include "y/memory/embedded.hpp"
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
            typedef Layout1D LayoutType;

            inline explicit In1D(LayoutType         layout,
                                 Memory::Allocator &memmgr) :
            LayoutType(layout),
            addr(0),
            emb1(addr,width),
            xemb( &emb1, 1, memmgr),
            make(addr,width)
            {
                addr -= lower;
            }

            //! directly assign aliens to address
            inline In1D(const LayoutType &layout,
                        MutableType      *aliens) noexcept :
            LayoutType(layout),
            addr(aliens-lower),
            emb1(addr,width),
            xemb(),
            make()
            {

            }

            inline virtual ~In1D() noexcept
            {

            }

            inline MutableType & operator[](const Coord1D q) noexcept
            {
                assert(q>=lower); assert(q<=upper);
                return addr[q];
            }

            inline ConstType & operator[](const Coord1D q) const noexcept
            {
                assert(q>=lower); assert(q<=upper);
                return addr[q];
            }

            inline std::ostream & display(std::ostream &os) const
            {
                return Core::Display(os, addr+lower, width);
            }

        private:
            MutableType *              addr;
            Memory::Embed              emb1;
            Memory::Embedded           xemb;
            MemoryBuilder<MutableType> make;

            Y_DISABLE_COPY_AND_ASSIGN(In1D);
        };


    }

}

#endif

