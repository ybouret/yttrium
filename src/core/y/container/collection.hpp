
//! \file

#ifndef Y_Container_Collection_Included
#define Y_Container_Collection_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{


    //__________________________________________________________________________
    //
    //
    //
    //! Collection base class
    //
    //
    //__________________________________________________________________________
    class Collection
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const char LBrack    = '['; //!< for display
        static const char RBrack    = ']'; //!< for display
        static const char SemiColon = ';'; //!< for display

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Collection() noexcept;
    public:
        virtual ~Collection() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual size_t size()  const noexcept = 0;
        unit_t         ssize() const;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Collection);
    };
}

#endif

