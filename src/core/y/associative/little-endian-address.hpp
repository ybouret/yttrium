
//! \file

#ifndef Y_Associative_LE_Address_Included
#define Y_Associative_LE_Address_Included 1

#include "y/associative/little-endian-key.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! Encode an address to a Big Endian R/O buffer
    //
    //__________________________________________________________________________
    class Little_Endian_Address : public Little_Endian_Key
    {
    public:

        template <typename T> inline
        explicit Little_Endian_Address(const T &args)        noexcept : Little_Endian_Key( &args )  {}  //!< setup
        virtual ~Little_Endian_Address()                     noexcept;                       //!< cleanup
        Little_Endian_Address(const Little_Endian_Address &) noexcept;                       //!< copy

        void *fetch() const noexcept; //!< retrieve stored address




    private:
        Y_DISABLE_ASSIGN(Little_Endian_Address);
    };

}

#endif
