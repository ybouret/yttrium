
//! \file

#ifndef Y_Container_Interface_Included
#define Y_Container_Interface_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    class Container
    {
    protected:
        explicit Container() noexcept;

    public:
        virtual ~Container() noexcept;


        virtual size_t size()      const noexcept = 0;
        virtual size_t capacity()  const noexcept = 0;
        virtual size_t available() const noexcept; //!< default to capacity() - size()
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Container);
    };
}

#endif
