//! \file

#ifndef Y_Type_Identifiable_Included
#define Y_Type_Identifiable_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Identifiable class
    //
    //
    //__________________________________________________________________________
    class Identifiable
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Identifiable() noexcept; //!< setup
        
    public:
        virtual ~Identifiable() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // methods
        //
        //______________________________________________________________________
        virtual const char * callSign() const noexcept = 0; //!< system wide call sign

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Identifiable);
    };
}

#endif

