//! \file

#ifndef Y_Chemical_Extents_Included
#define Y_Chemical_Extents_Included 1

#include "y/chemical/plexus/equalizer/extent.hpp"
#include "y/chemical/reactive/components.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        enum Resultant
        {
            Correct,
            BadReac,
            BadProd,
            BadBoth
        };

        const char * ResultantText(const Resultant) noexcept;

        //______________________________________________________________________
        //
        //
        //
        //! classified extents for both sides of an equilibrium
        //
        //
        //______________________________________________________________________
        class Extents : public Restartable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Extents(const EqzBanks &banks) noexcept; //!< setup empty
            virtual ~Extents() noexcept;                      //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! restart all
            virtual void restart() noexcept;

            //! dispatch all
            Resultant operator()(const Components &  E,
                                 const XReadable &   C,
                                 const Level         L,
                                 const AddressBook * const wanders);



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Extent   reac; //!< status for reactant(s)
            Extent   prod; //!< status for product(s)
            Boundary best; //!< best effort

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Extents);
            void findBest(const Boundary &limiting,
                          const Cursors  &requried);
        };

    
    }

}

#endif

