

//! \file

#ifndef Y_Calculus_Base10_Included
#define Y_Calculus_Base10_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //! base10 operations
    //
    //__________________________________________________________________________
    struct Base10
    {
        //! digits to represent u in base10
        static unsigned DigitsForU64(uint64_t u) noexcept;

        //! digits to represent u in base10
        template <typename T> static inline
        unsigned DigitsFor(const T u) noexcept
        {
            return DigitsForU64(u);
        }

        //! "C" string format '%0xxu', max to '%020u'
        class Format
        {
        public:
            Format(const uint64_t u)       noexcept; //!< setup from digits for u
            ~Format()                      noexcept; //!< cleanup
            Format(const Format &)         noexcept; //!< copy
            const char * operator*() const noexcept; //!< access format

        private:
            Y_DISABLE_ASSIGN(Format);
            char buffer[8];
        };


    };
}

#endif

