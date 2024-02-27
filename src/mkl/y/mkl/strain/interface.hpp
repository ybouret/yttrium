
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

            class Interface : public Recyclable
            {
            public:

                virtual size_t load() const noexcept = 0; //!< current load

                virtual ~Interface() noexcept;
            protected:
                explicit Interface() noexcept;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Interface);
            };


        }

    }

}

#endif
