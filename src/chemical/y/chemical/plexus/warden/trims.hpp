
//! \file

#ifndef Y_Chemical_Warden_Trims_Included
#define Y_Chemical_Warden_Trims_Included 1

#include "y/chemical/plexus/warden/trim.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Pair of Trims for reactants and products
        //
        //
        //______________________________________________________________________
        class Trims : public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxArray<Trims,XMemory> Array; //!< alias

            //! possible status
            enum Kind
            {
                BadNone, //!< no required
                BadReac, //!< reactants are required, products are ok
                BadProd, //!< products are required, reactants are ok
                BadBoth  //!< reactants and products are required
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Trims(const Fund &) noexcept; //!< setup
            virtual ~Trims()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! [Recyclable] free all
            virtual void free() noexcept;
            
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compute trims and return status
            /**
             \param C concentrations
             \param L level
             \param E equilibrium
             \param conserved database of conserved species
             */
            Kind operator()(const XReadable   &C,
                            const Level        L,
                            const Components  &E,
                            const AddressBook &conserved);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Trim           reac; //!< limiting/required reactants
            Trim           prod; //!< liimitin/required products

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Trims);
        };

    }

}

#endif

