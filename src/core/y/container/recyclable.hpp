

//! \file

#ifndef Y_Container_Recyclable_Included
#define Y_Container_Recyclable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Recyclable class
    //
    //
    //__________________________________________________________________________
    class Recyclable
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Recyclable() noexcept; //!< setup

    public:
        virtual ~Recyclable() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual void free()       noexcept = 0; //!< keep possible memory

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Recyclable);
    };

}

#endif
