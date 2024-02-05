
//! \file

#ifndef Y_Field4D_Included
#define Y_Field4D_Included 1

#include "y/field/3d.hpp"
#include "y/field/layout/4d.hpp"
#include "y/memory/embedding/quad.hpp"

namespace Yttrium
{
    namespace Field
    {
        //______________________________________________________________________
        //
        //
        //
        //! 4D  Field
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class In4D : public Sketch, public Proxy<const Layout4D>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type);                             //!< aliases
            typedef MetaKeyWith<0>             SelfMetaKey;  //!< alias
            typedef Sub1D<3,Type>              RowType;      //!< alias
            typedef Sub2D<2,Type>              SliceType;    //!< alias
            typedef Sub3D<1,Type>              VolumeType;   //!< alias
            typedef MemoryBuilder<VolumeType>  SelfBuilder;  //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! top-level constructor
            template <typename LABEL>
            inline explicit In4D(const LABEL    & label,
                                 const Format4D & space) :
            Sketch(),
            layout(space),
            metaKey(label),
            vol(0),
            slc(0),
            row(0),
            ptr(0),
            in3D( new Layout3D(SubLayout,*layout) ),
            in2D( new Layout2D(SubLayout,*layout) ),
            in1D( new Layout1D(SubLayout,*layout) ),
            plan(vol,layout->width.w,
                 slc,layout->width.w * layout->width.z,
                 row,layout->width.w * layout->width.z * layout->width.y,
                 ptr,layout->items),
            hold(plan,ALLOCATOR::Instance()),
            make(vol,layout->width.w,
                 metaKey,layout->lower.w,
                 in3D,in2D,in1D,
                 slc,row,ptr)
            {
                vol -= layout->lower.w;
            }

            //! cleanup
            inline virtual ~In4D() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! access
            inline VolumeType & operator[](const unit_t l) noexcept
            {
                assert(l>=layout->lower.w);
                assert(l<=layout->upper.w);
                return vol[l];
            }

            //! access, const
            inline const VolumeType & operator[](const unit_t l) const noexcept
            {
                assert(l>=layout->lower.w);
                assert(l<=layout->upper.w);
                return vol[l];
            }


            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            inline virtual const MetaKey & key() const noexcept { return metaKey;    } //!< get key
            inline virtual size_t          ram() const noexcept { return hold.bytes; } //!< get ram


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            const Format4D    layout;  //!< layout
            const SelfMetaKey metaKey; //!< meta key

        private:
            VolumeType             *vol;
            SliceType              *slc;
            RowType                *row;
            MutableType            *ptr;
        public:
            const Format3D          in3D; //!< 3D sub-layout
            const Format2D          in2D; //!< 2D sub-layout
            const Format1D          in1D; //!< 1D sub-layout
        private:
            Memory::Embedding::Quad plan;
            const Memory::Embedded  hold;
            SelfBuilder             make;
            
            Y_DISABLE_COPY_AND_ASSIGN(In4D);
            inline virtual ConstInterface & surrogate() const noexcept { return *layout; }
        };

    }
}

#endif
