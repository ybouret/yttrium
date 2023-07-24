
//! \file

#ifndef Y_Container_Extensible_Included
#define Y_Container_Extensible_Included 1

#include "y/type/releasable.hpp"

namespace Yttrium
{

    class Extensible : public Releasable
    {
    protected:
        explicit Extensible() noexcept;

    public:
        virtual ~Extensible() noexcept;

        virtual void free() noexcept = 0;
        virtual void reserve(size_t) = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Extensible);
    };

}

#endif
