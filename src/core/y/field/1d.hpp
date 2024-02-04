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

namespace Yttrium
{
    namespace Field
    {
       
#if 0
        //! Generic 1D Field
        template <size_t NSUB, typename T>
        class Sub1D : public Interface, public Layout1D
        {
        public:
            Y_ARGS_DECL(T,Type);                             //!< aliases
            typedef MetaKeyWith<NSUB>          SelfMetaKey;  //!< alias
            typedef Memory::EmbeddingSolo      SelfPattern;  //!< alias
            typedef Memory::Embedded           SelfAcquire;  //!< alias
            typedef MemoryBuilder<MutableType> SelfBuilder;
           
            //! default constructor
            template <typename LABEL>
            inline explicit Sub1D(const LABEL       & label,
                                  const Layout1D    & layout,
                                  Memory::Allocator & alloc) :
            Interface(),
            Layout1D(layout),
            metaKey(label),
            entry(0),
            motif(entry,items),
            owned(motif,alloc),
            inner(entry,items)
            {
                entry -= lower;
            }

            //! constructor as sub-field
            inline explicit Sub1D(const MetaKeyWith<NSUB-1> & rootKey,
                                  const unit_t                subIndx,
                                  const Layout1D            & layout,
                                  MutableType * const         aliens) :
            Interface(),
            Layout1D(layout),
            metaKey(rootKey,subIndx),
            entry(0),
            motif(entry,items),
            owned(),
            inner(entry=aliens,items)
            {
                entry  -= lower;
            }

            //! cleanup
            inline virtual ~Sub1D() noexcept { entry = 0; }

            // Interface

            //! get key
            inline virtual const MetaKey & key()                    const noexcept { return metaKey; }

            // Methods

            inline bool contains(const unit_t i) const noexcept { return (i>=lower) && (i<=upper); }

            inline ConstType & operator[](const unit_t i) const noexcept { assert(contains(i)); return entry[i]; }
            inline Type &      operator[](const unit_t i)       noexcept { assert(contains(i)); return entry[i]; }

            inline std::ostream & display(std::ostream &os) const
            {
                return Core::Display(os, entry+lower, width);
            }

            const SelfMetaKey metaKey;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub1D);
            MutableType *entry;
            SelfPattern  motif;
            SelfAcquire  owned;
            SelfBuilder  inner;
        };

        template <typename T, typename ALLOCATOR>
        class In1D : public Sub1D<0,T>
        {
        public:
            template <typename LABEL>
            inline explicit In1D(const LABEL &  label,
                                 const Layout1D layout) :
            Sub1D<0,T>(label,layout,ALLOCATOR::Instance())
            {
            }


            inline virtual ~In1D() noexcept {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(In1D);
        };
#endif
        
    }

}

#endif

