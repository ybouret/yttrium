//! \file

#ifndef Y_Field1D_Included
#define Y_Field1D_Included 1

#include "y/field/sketch.hpp"
#include "y/field/meta-key/with.hpp"
#include "y/field/memory/builder.hpp"
#include "y/memory/embedding/solo.hpp"
#include "y/memory/embedded.hpp"
#include "y/memory/allocator.hpp"
#include "y/type/args.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/proxy.hpp"

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
         - NSUB = 0: Standalone Field  => label
         - NSUB = 1: Row of a 2D space => label[j]
         - NSUB = 2: Row of a 3D space => label[k][j]
         - NSUB = 3: Row of a 4D space => label[t][k][k]
         */
        //
        //______________________________________________________________________
        template <size_t NSUB, typename T>
        class Sub1D : public Sketch, public Proxy<const Layout1D>
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

                Memory::Embedding::Solo  plan;
                const Memory::Embedded   hold;

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
            //! STANDALONE constructor, NSUB=0
            /**
             \param label  the label
             \param space  the layout
             \param alloc  the allocator
             */
            //__________________________________________________________________
            template <typename LABEL>
            inline explicit Sub1D(const LABEL       & label,
                                  const Format1D    & space,
                                  Memory::Allocator & alloc) :
            Sketch(),
            layout( space),
            metaKey(label),
            entry(0),
            code( new Code(alloc,entry,layout->items) ),
            make(entry,layout->items)
            {
                entry -= layout->lower;
            }

        public:
            //__________________________________________________________________
            //
            //
            //! constructor as sub-field, with PROVIDED memory for items
            /**
             \param rootKey key of the 2D space
             \param rowIndx row index in the 2D space
             \param space   1D layout
             \param alien   linear space for items
             */
            //
            //__________________________________________________________________
            inline explicit Sub1D(const MetaKeyWith<NSUB-1> & rootKey,
                                  const unit_t                rowIndx,
                                  const Format1D            & space,
                                  MutableType * const         alien) :
            Sketch(),
            layout(space),
            metaKey(rootKey,rowIndx),
            entry(alien),
            code(0),
            make(entry,layout->items)
            {
                entry  -= layout->lower;
            }

            //! cleanup
            inline virtual ~Sub1D() noexcept { ; }

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
            
            //! access, const
            inline ConstType & operator[](const unit_t i) const noexcept
            { 
                assert(i>=layout->lower); assert(i<=layout->upper);
                return entry[i];
            }

            //! access
            inline Type & operator[](const unit_t i) noexcept {
                assert(i>=layout->lower); assert(i<=layout->upper);
                return entry[i];
            }

            //! diplay constant
            inline std::ostream & display(std::ostream &os) const
            {
                return Core::Display(os, entry+layout->lower, layout->width);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Format1D    layout;  //!< shared layout
            const SelfMetaKey metaKey; //!< meta key

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub1D);
            MutableType              *entry;
            const AutoPtr<const Code> code;
            const SelfBuilder         make;;

            inline virtual ConstInterface & surrogate() const noexcept { return *layout; }
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
            inline explicit In1D(const LABEL    & label,
                                 const Format1D & space) :
            Sub1D<0,T>(label,space,ALLOCATOR::Instance())
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

