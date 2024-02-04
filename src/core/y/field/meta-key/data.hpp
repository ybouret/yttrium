//! \file

#ifndef Y_Field_Meta_Key_Data_Included
#define Y_Field_Meta_Key_Data_Included 1

#include "y/field/meta-key.hpp"

namespace Yttrium
{
    namespace Field
    {
        //______________________________________________________________________
        //
        //
        //
        //! Data for MetaKey
        //
        //
        //______________________________________________________________________
        template <size_t NSUB> class MetaKeyData
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t Maxi     = 1+NSUB;                      //!< Max SharedString
            static const size_t Required = Maxi * sizeof(SharedString); //!< internal bytes

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual      ~MetaKeyData() noexcept { clr(); } //!< cleanup
        protected:
            inline explicit      MetaKeyData() noexcept : wksp() { clr(); }  //!< setup
            inline virtual void *data()        noexcept { return &wksp[0]; } //!< access

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MetaKeyData);
            void *wksp[ Y_WORDS_GEQ(Required) ];
            inline void clr() noexcept { Y_STATIC_ZARR(wksp); }
        };
    }

}

#endif
