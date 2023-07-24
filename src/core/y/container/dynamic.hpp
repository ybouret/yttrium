
//! \file

#ifndef Y_Container_Dyanmic_Included
#define Y_Container_Dyanmic_Included 1

#include "y/container/interface.hpp"
#include "y/container/extensible.hpp"


namespace Yttrium
{

    class Dynamic : public Container, public Extensible
    {
    protected:
        explicit Dynamic() noexcept;

    public:
        virtual ~Dynamic() noexcept;

        void ensure(const size_t minCapacity);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dynamic);
    };

}

#endif
