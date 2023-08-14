
//! \file

#ifndef Y_Associative_BE_Addr_Included
#define Y_Associative_BE_Addr_Included 1

#include "y/associative/be-key.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! Encode an address to a Big Endian R/O buffer
    //
    //__________________________________________________________________________
    class BE_Addr : public BE_Key
    {
    public:

        template <typename T> inline
        explicit BE_Addr(const T &args) noexcept : BE_Key( &args )  {}  //!< setup
        virtual ~BE_Addr()              noexcept;                       //!< cleanup
        BE_Addr(const BE_Addr &)        noexcept;                       //!< copy

        const void *fetch() const noexcept; //!< retrieve stored address

    private:
        Y_DISABLE_ASSIGN(BE_Addr);
    };

}

#endif
