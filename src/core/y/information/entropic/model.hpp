
//! \file

#ifndef Y_Information_Entropic_Model_Included
#define Y_Information_Entropic_Model_Included 1

#include "y/information/entropic/unit.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {

            class Model
            {
            public:
                virtual ~Model() noexcept;

            protected:
                explicit Model() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Model);
            };

        }

    }

}

#endif

