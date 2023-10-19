
//! \file

#ifndef Y_Chemical_Species_Tier_Included
#define Y_Chemical_Species_Tier_Included 1

#include "y/chem/species.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! Tier of species
        //
        //______________________________________________________________________
        class Tier : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Tier() noexcept; //!< setup empty
            virtual ~Tier() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const AddressBook conserved; //!< conserved species
            const AddressBook unbounded; //!< unbounded species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tier);
        };
    }

}

#endif
