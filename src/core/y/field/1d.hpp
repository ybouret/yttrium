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
        template <typename T, size_t NSUB>
        class In1D : public Interface, public Layout1D
        {
        public:
            Y_ARGS_DECL(T,Type);                         //!< aliases
            typedef MetaKeyWith<NSUB>      SelfMetaKey;  //!< alias
            typedef Memory::EmbeddingSolo  SelfPattern;  //!< alias
            typedef Memory::Embedded       SelfAcquire;  //!< alias

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
            owned(motif,alloc)
            {
                
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
            inline virtual ~In1D() noexcept {}

            //! get key
            inline virtual const MetaKey & key() const noexcept { return metaKey; }



            const SelfMetaKey metaKey;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(In1D);
            MutableType *entry;
            SelfPattern  motif;
            SelfAcquire  owned;

        };

        
    }

}

#endif

