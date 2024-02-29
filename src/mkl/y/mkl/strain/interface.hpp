
//! \file

#ifndef Y_MKL_Strain_Interface_Included
#define Y_MKL_Strain_Interface_Included 1

#include "y/container/recyclable.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            //__________________________________________________________________
            //
            //
            //
            //! base class for filters
            //
            //
            //__________________________________________________________________
            class Interface : public Recyclable
            {
            public:
                static  const size_t SIZE = 3;
                virtual size_t load() const noexcept = 0; //!< current load

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
