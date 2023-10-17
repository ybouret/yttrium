
//! \file

#ifndef Y_Chemical_Conservation_Canon_Included
#define Y_Chemical_Conservation_Canon_Included 1

#include "y/chem/plexus/conservation.hpp"
#include "y/data/small/light/list/bare.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! light list of DEPENDENT conservations
        //
        //______________________________________________________________________
        typedef Small::BareLightList<const Conservation> Conservations;

        //______________________________________________________________________
        //
        //
        //
        //! list of dependent conservations
        //
        //
        //______________________________________________________________________
        class Canon : public Object, public Conservations
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Canon(const Conservation &first); //!< setup with first conservation
            virtual ~Canon() noexcept;                 //!< cleanup

            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool accepts(const Conservation &) const noexcept; //!< true if sharing a species
            bool accepts(const Canon &)        const noexcept; //!< true if sharing a species

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Canon *next; //!< for list
            Canon *prev; //!< for list
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Canon);
        };

    }

}

#endif

