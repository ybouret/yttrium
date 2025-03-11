
//! \file


#ifndef Y_Chemical_Charge_Included
#define Y_Chemical_Charge_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Charge to build Species
        //
        //
        //______________________________________________________________________
        class Charge
        {
            protected: explicit Charge() noexcept; //!< setup
            public:    virtual ~Charge() noexcept; //!< cleanup
            const int z;                           //!< algebraic charge

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Charge);
        };

    }

}

#endif

