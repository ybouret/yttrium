
//! \file

#ifndef Y_Field_Meta_Key_With_Included
#define Y_Field_Meta_Key_With_Included 1

#include "y/field/meta-key/data.hpp"

namespace Yttrium
{
    namespace Field
    {
        template <size_t NSUB>
        class MetaKeyWith : public MetaKeyData<NSUB>, public MetaKey
        {
        public:
            typedef MetaKeyData<NSUB> DataType; //!< alias


            inline explicit MetaKeyWith(const SharedString &uuid) noexcept:
            DataType(),
            MetaKey(this->data())
            {
                push(uuid);
            }


            inline explicit MetaKeyWith(const MetaKeyWith<NSUB-1> &root,
                                        const unit_t               indx) noexcept:
            DataType(),
            MetaKey(this->data())
            {
                static IndexDataBase &idb = IndexDataBase::Instance();
                const unsigned        num = root.size();
                for(unsigned i=0;i<num;++i) push( root[i] );
                push( idb[indx] );
            }

            inline virtual ~MetaKeyWith() noexcept {}

            // interface
            inline virtual unsigned maxi() const noexcept { return this->Maxi; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyWith);
        };

    }

}

#endif

