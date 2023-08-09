//! \file

#ifndef Y_IO_Serializable_Included
#define Y_IO_Serializable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    class OutputStream;

    //__________________________________________________________________________
    //
    //
    //
    //! Serializable interface
    //
    //
    //__________________________________________________________________________
    class Serializable
    {
    public:
        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! save data, return written bytes
        //______________________________________________________________________
        virtual size_t serialize(OutputStream &) const = 0;

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        virtual ~Serializable() noexcept; //!< cleanup
    protected:
        explicit Serializable() noexcept; //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Serializable);
    };
}

#endif

