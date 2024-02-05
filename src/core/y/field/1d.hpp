//! \file

#ifndef Y_Field1D_Included
#define Y_Field1D_Included 1

#include "y/field/interface.hpp"
#include "y/field/meta-key/with.hpp"
#include "y/field/memory/builder.hpp"
#include "y/memory/embedding/solo.hpp"
#include "y/memory/embedded.hpp"
#include "y/memory/allocator.hpp"
#include "y/type/args.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Field
    {
       
        //______________________________________________________________________
        //
        //
        //
        //! Generic 1D Subspace
        /**
         - NSUB = 0: Standalone Field
         - NSUB = 1: Row of a 2D space
         - NSUB = 2: Row of a 3D space
         - NSUB = 4: Row of a 4D space
         */
        //
        //______________________________________________________________________
        template <size_t NSUB, typename T>
        class Sub1D : public Interface, public Layout1D
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
            typedef MemoryBuilder<MutableType> SelfBuilder;  //!< alias

        private:
            class Code : public Object
            {
            public:
                template <typename U1>
                inline explicit Code(Memory::Allocator &ma,
                                     U1 *              &u1,
                                     const size_t       n1) :
                plan(u1,n1),
                hold(plan,ma) {}

                inline virtual ~Code() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
                Memory::Embedding::Solo  plan;
                Memory::Embedded         hold;
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

        protected:
            //__________________________________________________________________
            //! STANDALONE constructor, NSUB=0
            /**
             \param label  the label
             \param layout the layout
             \param alloc  the allocator
             */
            //__________________________________________________________________
            template <typename LABEL>
            inline explicit Sub1D(const LABEL       & label,
                                  const Layout1D    & layout,
                                  Memory::Allocator & alloc) :
            Interface(),
            Layout1D(layout),
            metaKey(label),
            entry(0),
            code( new Code(alloc,entry,items) ),
            inner(entry,items)
            {
                entry -= lower;
            }

        public:
            //__________________________________________________________________
            //
            //
            //! constructor as sub-field, with PROVIDED memory for items
            /**
             \param rootKey key of the 2D space
             \param rowIndx row index in the 2D space
             \param layout  1D layout
             \param aliens  linear space for itemds
             */
            //
            //__________________________________________________________________
            inline explicit Sub1D(const MetaKeyWith<NSUB-1> & rootKey,
                                  const unit_t                rowIndx,
                                  const Layout1D            & layout,
                                  MutableType * const         aliens) :
            Interface(),
            Layout1D(layout),
            metaKey(rootKey,rowIndx),
            entry(aliens),
            code(0),
            inner(entry,items)
            {
                entry  -= lower;
            }

            //! cleanup
            inline virtual ~Sub1D() noexcept { entry = 0; }

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            
            //! get key
            inline virtual const MetaKey & key() const noexcept { return metaKey; }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            
            //! access, const
            inline ConstType & operator[](const unit_t i) const noexcept
            { 
                assert(i>=lower); assert(i<=upper);
                return entry[i];
            }

            //! access
            inline Type & operator[](const unit_t i) noexcept {
                assert(i>=lower); assert(i<=upper);
                return entry[i];
            }

            //! diplay constant
            inline std::ostream & display(std::ostream &os) const
            {
                return Core::Display(os, entry+lower, width);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const SelfMetaKey metaKey; //!< meta key

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub1D);
            MutableType              *entry;
            const AutoPtr<const Code> code;
            SelfBuilder               inner;
        };

        //______________________________________________________________________
        //
        //
        //
        //! Standalone 1D field
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR>
        class In1D : public Sub1D<0,T>
        {
        public:

            //! setup
            template <typename LABEL>
            inline explicit In1D(const LABEL &  label,
                                 const Layout1D layout) :
            Sub1D<0,T>(label,layout,ALLOCATOR::Instance())
            {
            }


            //! cleanup
            inline virtual ~In1D() noexcept {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(In1D);
        };

    }

}

#endif

