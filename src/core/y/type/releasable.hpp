
//! \file

#ifndef Y_Type_Releasable_Included
#define Y_Type_Releasable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    class Releasable
    {

    protected:
        explicit Releasable() noexcept;

    public:
        virtual ~Releasable() noexcept;
        virtual void release() noexcept  = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Releasable);
    };

}

#endif
