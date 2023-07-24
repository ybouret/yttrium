
//! \file

#ifndef Y_Container_Collection_Included
#define Y_Container_Collection_Included 1

#include "y/type/identifiable.hpp"

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
    class Collection : public virtual Identifiable
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
        explicit Collection() noexcept; //!< setup
    public:
        virtual ~Collection() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual size_t size()  const noexcept = 0; //!< objects in collection
        unit_t         ssize() const;              //!< convert to signed type

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Collection);
    };
}

#endif

