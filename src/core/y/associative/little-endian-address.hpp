
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
    //
    //! Encode an address to a Big Endian R/O buffer
    //
    //
    //__________________________________________________________________________
    class LittleEndianAddress : public LittleEndianKey
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        template <typename T> inline
        LittleEndianAddress(const T &args)        noexcept : LittleEndianKey( &args )  {}  //!< setup
        LittleEndianAddress(const CopyOf_ &, const void *addr) noexcept;                   //!< setup
        virtual ~LittleEndianAddress()                     noexcept;                       //!< cleanup
        LittleEndianAddress(const LittleEndianAddress &)   noexcept;                       //!< copy

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        void *fetch() const noexcept; //!< retrieve stored address




    private:
        Y_DISABLE_ASSIGN(LittleEndianAddress);
    };

}

#endif
