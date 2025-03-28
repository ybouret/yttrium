
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            //! Internal list of laws
            //
            //
            //__________________________________________________________________
            class Canon : public Law::List, public Assembly
            {
            public:
                explicit Canon() noexcept; //!< setup
                virtual ~Canon() noexcept; //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canon);
            };
        }

    }

}

#endif

