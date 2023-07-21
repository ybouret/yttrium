
//! \file

#ifndef Y_Container_Interface_Included
#define Y_Container_Interface_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{


    //__________________________________________________________________________
    //
    //
    //
    //! Container interface
    //
    //
    //__________________________________________________________________________
    class Container
    {

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
    protected:
        explicit Container() noexcept; //!< setup

    public:
        virtual ~Container() noexcept; //!< cleanup



        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual size_t size()      const noexcept = 0; //!< items in conatiner
        virtual size_t capacity()  const noexcept = 0; //!< maximum items in container
        virtual size_t available() const noexcept;     //!< default to capacity() - size(), can be overiden
        static size_t NextCapacity(const size_t n);    //!< evaluate next capacity



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Container);
    };
}

#endif
