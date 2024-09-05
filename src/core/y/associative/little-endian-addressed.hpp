
//! \file

#ifndef Y_Associative_LE_Addressed_Included
#define Y_Associative_LE_Addressed_Included 1

#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Base class to use unique key based on object address
    //
    //
    //__________________________________________________________________________
    class LittleEndianAddressed
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit LittleEndianAddressed() noexcept; //!< setup with *this
        virtual ~LittleEndianAddressed() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const LittleEndianAddress leak; //!< unique key based on address

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LittleEndianAddressed);
    };
}

#endif

