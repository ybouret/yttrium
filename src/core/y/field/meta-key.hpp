
//! \file

#ifndef Y_Field_Meta_Key_Included
#define Y_Field_Meta_Key_Included 1

#include "y/field/idb.hpp"

namespace Yttrium
{
    namespace Field
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! MetaKey interface
        //
        //
        //______________________________________________________________________
        class MetaKey
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual unsigned maxi()                     const noexcept = 0; //!< capacity
            unsigned         size()                     const noexcept;     //!< stored strings (a.k.a dimensions)
            const String &   operator[](const unsigned) const noexcept;     //!< access in [0..size()-1]

            //__________________________________________________________________
            //
            //
            // Operators
            //
            //__________________________________________________________________
            friend bool operator==(const MetaKey &lhs, const MetaKey &rhs) noexcept;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Y_OSTREAM_PROTO(MetaKey);                          //!< display
            virtual ~MetaKey()                      noexcept;  //!< cleanup
        protected:
            explicit MetaKey(void * const data)     noexcept;  //!< setup (from MetaKeyData)

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void     push(const SharedString &uuid) noexcept; //!< store a shared string
            void     push(const String &s)          noexcept; //!< store from database, s.quantity()>0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKey);
            unsigned             built;
            SharedString * const array;

        };

     

      
    }

}


#endif

