//! \file

#ifndef Y_Type_Zeroed_Field_Included
#define Y_Type_Zeroed_Field_Included 1

#include "y/memory/out-of-reach.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Field of zeroed data (POD-like only objects)
    //
    //
    //__________________________________________________________________________
    template <typename T>
    class ZeroedField
    {
    public:
        inline  ZeroedField() noexcept : wksp() { ldz(); } //!< setup
        inline ~ZeroedField() noexcept          { }        //!< cleanup

        //! access
        const T & operator*() const noexcept
        {
            assert( Memory::OutOfReach::Are0(wksp, sizeof(wksp)) );
            return *static_cast<const T *>(Memory::OutOfReach::Addr(wksp));
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(ZeroedField);
        void *wksp[ Y_WORDS_FOR(T) ];

        inline void ldz() noexcept { (void)Y_STATIC_ZARR(wksp); }
    };

}

#endif
