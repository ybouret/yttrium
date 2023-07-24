
//! \file

#ifndef Y_Container_Dynamic_Included
#define Y_Container_Dynamic_Included 1

#include "y/type/releasable.hpp"

namespace Yttrium
{

    class Dynamic : public Releasable
    {
    protected:
        explicit Dynamic() noexcept;

    public:
        virtual ~Dynamic() noexcept;

        virtual void free() noexcept = 0;
        virtual void reserve(size_t) = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dynamic);
    };

}

#endif
