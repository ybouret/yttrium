//! \file

#ifndef Y_Type_Identifiable_Incldued
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

    private:
        virtual const char * callSign() const noexcept = 0;

    };
}

#endif

