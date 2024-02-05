
//! \file

#ifndef Y_Field2D_Included
#define Y_Field2D_Included 1

#include "y/field/1d.hpp"
#include "y/field/layout/2d.hpp"
#include "y/memory/embedding/pair.hpp"

namespace Yttrium
{
    namespace Field
    {

        //! final call to make field
#define Y_FIELD2D_MAKE()  make(row,layout->numRows(),metaKey,layout->lower.y,in1D,addr)

        //______________________________________________________________________
        //
        //
        //
        //! Generic 2D Subspace
        /**
         - NSUB = 0: Standalone Field     => label
         - NSUB = 1: Slice of a 3D space  => label[k]
         - NSUB = 2: Slice of a 4D space  => label[t][k]
         */
        //
        //______________________________________________________________________
        template <size_t NSUB, typename T>
        class Sub2D : public Sketch, public Proxy<const Layout2D>
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
            typedef Sub1D<NSUB+1,Type>         RowType;      //!< alias
            typedef MemoryBuilder<RowType>     SelfBuilder;  //!< alias

        private:
            class Code : public Object
            {
            public:
                template <typename U1, typename U2>
                inline explicit Code(Memory::Allocator &ma,
                                     U1 *              &u1,
                                     const size_t       n1,
                                     U2 *              &u2,
                                     const size_t       n2) :
                plan(u1,n1,u2,n2),
                hold(plan,ma) {}

                inline virtual ~Code() noexcept {}

                Memory::Embedding::Pair plan;
                const Memory::Embedded  hold;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:

            //__________________________________________________________________
            //
            //! STANDALONE constructor, NSUB=0
            //__________________________________________________________________
            template <typename LABEL>
            inline explicit Sub2D(const LABEL       & label,
                                  const Format2D    & space,
                                  Memory::Allocator & alloc) :
            Sketch(),
            layout(space),
            metaKey(label),
            row(0),
            addr(0),
            in1D( new Layout1D(SubLayout,*layout) ),
            code( new Code(alloc,row,layout->width.y,addr,layout->items) ),
            Y_FIELD2D_MAKE()
            {
                row -= layout->lower.y;
            }

        public:
            //__________________________________________________________________
            //
            //! Construct as a slice of higher space
            /**
             \param rootKey   key of [3|4]D Field
             \param slcIndx   index of slice
             \param space2D   shared layout of this slice
             \param space1D   shared layout of rows
             \param alienRows allocated memory for space2D->width.y
             \param alienData allocated memory for space2D->items
             */
            //__________________________________________________________________
            explicit Sub2D(const MetaKeyWith<NSUB-1> & rootKey,
                           const unit_t                slcIndx,
                           const Format2D            & space2D,
                           const Format1D            & space1D,
                           RowType                   * alienRows,
                           MutableType               * alienData) :
            Sketch(),
            layout(space2D),
            metaKey(rootKey,slcIndx),
            row(  alienRows ),
            addr( alienData ),
            in1D( space1D   ),
            code(0),
            Y_FIELD2D_MAKE()
            {
                row -= layout->lower.y;
            }

            inline virtual ~Sub2D() noexcept { row=0; }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get row
            inline RowType & operator[](const unit_t j) noexcept
            {
                assert(j>=layout->lower.y); assert(j<=layout->upper.y); return row[j];
            }

            //! get row, const
            inline const RowType & operator[](const unit_t j) const noexcept
            {
                assert(j>=layout->lower.y); assert(j<=layout->upper.y); return row[j];
            }

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
            // Members
            //
            //__________________________________________________________________
            const Format2D    layout;  //!< shared layout
            const SelfMetaKey metaKey; //!< meta key

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub2D);
            RowType      *            row;    //!< row, offseted
            MutableType  *            addr;   //!< linear space of items
        public:
            const Format1D            in1D;   //!< shared 1D layout
        private:
            const AutoPtr<const Code> code;   //!< for private data
            const SelfBuilder         make;   //!< build with row/addr

            inline virtual ConstInterface & surrogate() const noexcept { return *layout; }
        };


        //______________________________________________________________________
        //
        //
        //
        //! Standalone 2D field
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class In2D : public Sub2D<0,T>
        {
        public:

            //! setup
            template <typename LABEL>
            inline explicit In2D(const LABEL    & label,
                                 const Format2D & space) :
            Sub2D<0,T>(label,space, ALLOCATOR::Instance() )
            {
            }

            //! cleanup
            inline virtual ~In2D() noexcept {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(In2D);
        };

    }

}

#endif

