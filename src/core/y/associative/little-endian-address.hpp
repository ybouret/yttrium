
//! \file

#ifndef Y_Associative_LE_Address_Included
#define Y_Associative_LE_Address_Included 1

#include "y/associative/little-endian-key.hpp"
#include "y/type/copy.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! Encode an address to a Big Endian R/O buffer
    //
    //__________________________________________________________________________
    class LittleEndianAddress : public LittleEndianKey
    {
    public:

        template <typename T> inline
        explicit LittleEndianAddress(const T &args)        noexcept : LittleEndianKey( &args )  {}  //!< setup
        explicit LittleEndianAddress(const CopyOf_ &, const void *addr) noexcept;                   //!< setup
        virtual ~LittleEndianAddress()                     noexcept;                                //!< cleanup
        LittleEndianAddress(const LittleEndianAddress &)   noexcept;                                //!< copy

        void *fetch() const noexcept; //!< retrieve stored address




    private:
        Y_DISABLE_ASSIGN(LittleEndianAddress);
    };

}

#endif
