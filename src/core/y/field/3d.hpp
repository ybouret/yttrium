//! \file

#ifndef Y_Field3D_Included
#define Y_Field3D_Included 1

#include "y/field/2d.hpp"
#include "y/field/layout/3d.hpp"
#include "y/memory/embedding/trio.hpp"

namespace Yttrium
{
    namespace Field
    {
        

        //! final call to make field
#define Y_FIELD3D_MAKE()  make(slc,layout->numSlices(),metaKey,layout->lower.z,in2D,in1D,row,ptr)

        //______________________________________________________________________
        //
        //
        //
        //! Generic 3D Sub-Field
        /**
         - NSUB = 0 : Standalone Field    => label
         - NSUB = 1 : Space of a 4D space => label[t]
         */
        //______________________________________________________________________
        template <size_t NSUB, typename T>
        class Sub3D : public Sketch, public Proxy<const Layout3D>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_DECL(T,Type);                             //!< aliases
            typedef MetaKeyWith<NSUB>          SelfMetaKey;  //!< alias
            typedef Sub1D<NSUB+2,Type>         RowType;      //!< alias
            typedef Sub2D<NSUB+1,Type>         SliceType;    //!< alias
            typedef MemoryBuilder<SliceType>   SelfBuilder;  //!< alias

        private:
            class Code : public Object
            {
            public:
                template <typename U1, typename U2, typename U3>
                inline explicit Code(Memory::Allocator &ma,
                                     U1 *              &u1,
                                     const size_t       n1,
                                     U2 *              &u2,
                                     const size_t       n2,
                                     U3 *              &u3,
                                     const size_t       n3
                                     ) :
                plan(u1,n1,u2,n2,u3,n3),
                hold(plan,ma) {}

                inline virtual ~Code() noexcept {}

                Memory::Embedding::Trio plan;
                const Memory::Embedded  hold;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

        protected:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! STANDALONE constructor, NSUB=0
            //__________________________________________________________________
            template <typename LABEL>
            inline explicit Sub3D(const LABEL       & label,
                                  const Format3D    & space,
                                  Memory::Allocator & alloc) :
            Sketch(),
            layout(space),
            metaKey(label),
            slc( 0 ),
            row( 0 ),
            ptr( 0 ),
            in2D( new Layout2D(SubLayout,*layout) ),
            in1D( new Layout1D(SubLayout,*layout) ),
            code( new Code(alloc,slc,layout->numSlices(),row,layout->numRows(),ptr,layout->items) ),
            Y_FIELD3D_MAKE()
            {
                slc -= layout->lower.z;
            }

        public:

            //__________________________________________________________________
            //
            //! Construct as a volume of higher space
            /**
             \param rootKey   key of 4D Field
             \param volIndx   index of volume
             \param space3D   shared layout of this volume
             \param space2D   shared layout of slices
             \param space1D   shared layout of rows
             \param alienSlcs allocated memory for space3D->width.z
             \param alienRows allocated memory for space3D->width.y * space3D->width.z
             \param alienData allocated memory for space3D->items
             */
            //__________________________________________________________________
            explicit Sub3D(const MetaKeyWith<NSUB-1> & rootKey,
                           const unit_t                volIndx,
                           const Format3D            & space3D,
                           const Format2D            & space2D,
                           const Format1D            & space1D,
                           SliceType                 * alienSlcs,
                           RowType                   * alienRows,
                           MutableType               * alienData) :
            Sketch(),
            layout( space3D ),
            metaKey(rootKey,volIndx),
            slc(alienSlcs),
            row(alienRows),
            ptr(alienData),
            in2D(space2D),
            in1D(space1D),
            code(0),
            Y_FIELD3D_MAKE()
            {
                slc -= layout->lower.z;
            }

            //! cleanup
            inline virtual ~Sub3D() noexcept {}

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            inline virtual const MetaKey & key() const noexcept { return metaKey; }                               //!< get key
            inline virtual size_t          ram() const noexcept { return code.isValid() ? code->hold.bytes : 0; } //!< get ram


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! access
            inline SliceType & operator[](const unit_t k) noexcept
            {
                assert(k>=layout->lower.z);
                assert(k<=layout->upper.z);
                return slc[k];
            }

            //! access, const
            inline const SliceType & operator[](const unit_t k) const noexcept
            {
                assert(k>=layout->lower.z);
                assert(k<=layout->upper.z);
                return slc[k];
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Format3D    layout;    //!< shared 3D layout
            const SelfMetaKey metaKey;   //!< meta key

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub3D);
            SliceType   *             slc;  //!< slice address, shifted
            RowType     *             row;  //!< rows address
            MutableType *             ptr;  //!< items address
        public:
            const Format2D            in2D; //!< shared 2D layout
            const Format1D            in1D; //!< shared 1D layout
        private:
            const AutoPtr<const Code> code; //!< mapping
            const SelfBuilder         make; //!< builder of slices

            inline virtual ConstInterface & surrogate() const noexcept { return *layout; }
        };

        //______________________________________________________________________
        //
        //
        //
        //! Standalone 3D field
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class In3D : public Sub3D<0,T>
        {
        public:

            //! setup
            template <typename LABEL>
            inline explicit In3D(const LABEL    & label,
                                 const Format3D & space) :
            Sub3D<0,T>(label,space, ALLOCATOR::Instance() )
            {
            }

            //! cleanup
            inline virtual ~In3D() noexcept {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(In3D);
        };

    }



}

#endif


