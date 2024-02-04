
//! \file

#ifndef Y_Field_Meta_Key_Hasher_Included
#define Y_Field_Meta_Key_Hasher_Included 1

#include "y/field/meta-key.hpp"
#include "y/hashing/fnv.hpp"

namespace Yttrium
{
    namespace Field
    {

        //______________________________________________________________________
        //
        //
        //
        //! MetaKey Hasher
        //
        //
        //______________________________________________________________________
        class MetaKeyHasher
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit MetaKeyHasher() noexcept; //!< setup
            virtual ~MetaKeyHasher() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! iterative hashing over strings
            size_t operator()(const MetaKey &mk) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyHasher);
            Hashing::FNV h;
        };

    }

}

#endif
