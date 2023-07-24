//! \file

#ifndef Y_Type_Identifiable_Included
#define Y_Type_Identifiable_Included 1

#include "y/config/starting.hpp"


namespace Yttrium
{
    class Identifiable
    {
    protected:
        explicit Identifiable() noexcept;
        
    public:
        virtual ~Identifiable() noexcept;

        virtual const char * callSign() const noexcept = 0;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Identifiable);
    };
}

#endif

