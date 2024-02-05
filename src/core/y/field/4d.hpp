
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

            template <typename LABEL>
            inline explicit In4D(const LABEL    & label,
                                 const Format4D & space) :
            Sketch(),
            layout(space),
            metaKey(label),
            vol(0),
            slc(0),
            row(0),
            data(0),
            in3d( new Layout3D(SubLayout,*layout) ),
            in2d( new Layout2D(SubLayout,*layout) ),
            in1d( new Layout1D(SubLayout,*layout) )
            {

            }


            inline virtual ~In4D() noexcept
            {

            }

            const Format4D    layout;
            const SelfMetaKey metaKey;
        private:
            VolumeType    *vol;
            SliceType     *slc;
            RowType       *row;
            MutableType   *data;
            const Format3D in3d;
            const Format2D in2d;
            const Format1D in1d;


        };

    }
}

#endif
