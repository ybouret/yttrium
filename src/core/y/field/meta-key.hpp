
//! \file

#ifndef Y_Field_Meta_Key_Included
#define Y_Field_Meta_Key_Included 1

#include "y/field/idb.hpp"

namespace Yttrium
{
    namespace Field
    {
        class MetaKey
        {
        public:
            virtual unsigned maxi()                     const noexcept = 0;
            unsigned         size()                     const noexcept;
            const String &   operator[](const unsigned) const noexcept;


            Y_OSTREAM_PROTO(MetaKey);
            virtual ~MetaKey()                      noexcept;
        protected:
            explicit MetaKey(void * const data)     noexcept;
            void     push(const SharedString &uuid) noexcept; //!< store
            void     push(const String &s)          noexcept; //!< quantity>0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKey);
            unsigned             built;
            SharedString * const array;

        };

     

      
    }

}


#endif

