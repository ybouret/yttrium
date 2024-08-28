
//! \file

#ifndef Y_Field_Meta_Key_With_Included
#define Y_Field_Meta_Key_With_Included 1

#include "y/field/meta-key/data.hpp"

namespace Yttrium
{
    namespace Field
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! MetaKey With subspaces
        //
        //
        //______________________________________________________________________
        template <size_t NSUB>
        class MetaKeyWith : public MetaKeyData<NSUB>, public MetaKey
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef MetaKeyData<NSUB> DataType; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
           
            //! setup from parent
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

            //! cleanup
            inline virtual ~MetaKeyWith() noexcept {}

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! static capacity
            inline virtual unsigned maxi() const noexcept { return this->Maxi; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyWith);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Specialized Top-Level MetaKey
        //
        //
        //______________________________________________________________________
        template <>
        class MetaKeyWith<0> :public MetaKeyData<0>, public MetaKey
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef MetaKeyData<0> DataType; //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
          
            //! setup from shared string
            inline explicit MetaKeyWith(const SharedString &uuid) noexcept:
            DataType(),
            MetaKey(this->data())
            {
                push(uuid);
            }

            //! setup from regular string
            inline explicit MetaKeyWith(const String &uuid) :
            DataType(), MetaKey(this->data())
            {
                const SharedString temp = new String(uuid);
                push(temp);
            }

            //! setup from text
            inline explicit MetaKeyWith(const char *uuid) :
            DataType(), MetaKey(this->data())
            {
                const SharedString temp = new String(uuid);
                push(temp);
            }

            //! cleanup
            inline virtual ~MetaKeyWith() noexcept {} 

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! static capacity
            inline virtual unsigned maxi() const noexcept { return this->Maxi; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyWith);
        };
    }

}

#endif

