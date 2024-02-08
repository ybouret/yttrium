
//! \file

#ifndef Y_MKL_Antelope_Interface_Included
#define Y_MKL_Antelope_Interface_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {

            //__________________________________________________________________
            //
            //
            //! Agnostic interface for codes
            //
            //__________________________________________________________________
            class Interface
            {
            public:
                virtual bool   isEmpty()               const noexcept = 0; //!< empty or zero
                virtual bool   accepts(const size_t n) const noexcept = 0; //!< test capacity
                virtual size_t ability()               const noexcept = 0; //!< capacity or zero


                virtual ~Interface() noexcept; //!< cleanup
            protected:
                explicit Interface() noexcept; //!< setup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Interface);
            };

        }
    }

}

#endif
