//! \file

#ifndef Y_Field1D_Included
#define Y_Field1D_Included 1

#include "y/field/layout.hpp"
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
        typedef unit_t          Coord1D;  //!< alias
        typedef Layout<Coord1D> Layout1D; //!< alias

        //! Generic 1D Field
        template <size_t NSUB, typename T>
        class In1D : public Interface, public Layout1D
        {
        public:
            Y_ARGS_DECL(T,Type);                             //!< aliases
            typedef MetaKeyWith<NSUB>          SelfMetaKey;  //!< alias
            typedef Memory::EmbeddingSolo      SelfPattern;  //!< alias
            typedef Memory::Embedded           SelfAcquire;  //!< alias
            typedef MemoryBuilder<MutableType> SelfBuilder;
            //! default constructor
            template <typename LABEL>
            inline explicit In1D(const LABEL       & label,
                                 const Layout1D      layout,
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
            inline explicit In1D(const MetaKeyWith<NSUB-1> &rootKey,
                                 const unit_t               subIndx,
                                 const Layout1D            &layout) :
            Interface(),
            Layout1D(layout),
            metaKey(rootKey,subIndx)
            {
            }

            //! cleanup
            inline virtual ~In1D() noexcept { entry = 0; }

            //! get key
            inline virtual const MetaKey & key()                    const noexcept { return metaKey; }

            inline bool contains(const unit_t i) const noexcept { return (i>=lower) && (i<=upper); }

            inline ConstType & operator[](const unit_t i) const noexcept { assert(contains(i)); return entry[i]; }
            inline Type &      operator[](const unit_t i)       noexcept { assert(contains(i)); return entry[i]; }

            inline std::ostream & display(std::ostream &os) const
            {
                return Core::Display(os, entry+lower, width);
            }

            const SelfMetaKey metaKey;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(In1D);
            MutableType *entry;
            SelfPattern  motif;
            SelfAcquire  owned;
            SelfBuilder  inner;
        };

        
    }

}

#endif

