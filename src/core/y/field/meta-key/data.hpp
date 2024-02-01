//! \file

#ifndef Y_Field_Meta_Key_Data_Included
#define Y_Field_Meta_Key_Data_Included 1

#include "y/field/meta-key.hpp"

namespace Yttrium
{
    namespace Field
    {
        template <size_t NSUB> class MetaKeyData
        {
        public:
            static const size_t Maxi     = 1+NSUB;;
            static const size_t Required = Maxi * sizeof(SharedString);

            inline virtual      ~MetaKeyData() noexcept { clr(); }
        protected:
            inline explicit      MetaKeyData() noexcept : wksp() { clr(); }
            inline virtual void *data()        noexcept { return &wksp[0]; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyData);
            void *wksp[ Y_WORDS_GEQ(Required) ];
            inline void clr() noexcept { Y_STATIC_ZARR(wksp); }
        };
    }

}

#endif
