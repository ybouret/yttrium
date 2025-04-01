
//! \file

#ifndef Y_Chemical_Conservation_Canons_Included
#define Y_Chemical_Conservation_Canons_Included 1

#include "y/chemical/plexus/conservation/canon.hpp"

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
            //! Independant Canons of dependent Laws
            //
            //
            //__________________________________________________________________
            class Canons : public CxxListOf<Canon>
            {
            public:
                explicit Canons(XMLog &xml,
                                const ListOf<Law> &laws,
                                const EList &definite); //!< setup
                virtual ~Canons() noexcept;                                      //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Canons);
            };

        }

    }

}

#endif

