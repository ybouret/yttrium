
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
    protected:
        explicit Collection() noexcept;
    public:
        virtual ~Collection() noexcept;

        virtual size_t size()  const noexcept = 0;
        unit_t         ssize() const;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Collection);
    };
}

#endif

