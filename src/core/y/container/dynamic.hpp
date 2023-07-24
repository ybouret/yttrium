
//! \file

#ifndef Y_Container_Dyanmic_Included
#define Y_Container_Dyanmic_Included 1

#include "y/container/interface.hpp"
#include "y/container/extensible.hpp"


namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Extensible Container
    //
    //
    //__________________________________________________________________________
    class Dynamic : public Container, public Extensible
    {
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Dynamic() noexcept; //!< setup

    public:
        virtual ~Dynamic() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! ensure minimal capacity
        void ensure(const size_t minCapacity);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dynamic);
    };

}

#endif
