
//! \file

#ifndef Y_Field_Meta_Key_Hasher_Included
#define Y_Field_Meta_Key_Hasher_Included 1

#include "y/field/meta-key.hpp"
#include "y/hashing/fnv.hpp"

namespace Yttrium
{
    namespace Field
    {

        class MetaKeyHasher
        {
        public:
            explicit MetaKeyHasher() noexcept;
            virtual ~MetaKeyHasher() noexcept;

            size_t operator()(const MetaKey &mk) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyHasher);
            Hashing::FNV h;
        };

    }

}

#endif
