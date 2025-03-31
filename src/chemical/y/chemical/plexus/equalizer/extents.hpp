//! \file

#ifndef Y_Chemical_Extents_Included
#define Y_Chemical_Extents_Included 1

#include "y/chemical/plexus/equalizer/extent.hpp"
#include "y/chemical/reactive/components.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Extents analyze result
        //
        //______________________________________________________________________
        enum Resultant
        {
            Correct, //!< balanced
            BadReac, //!< at least one bad reactant, products are ok
            BadProd, //!< at least one bad product, reactants are ok
            BadBoth  //!< at least one bad on each side
        };


        //! Human Readable Resultant
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
            Resultant operator()(XMLog &             xml,
                                 const Components &  E,
                                 const XReadable &   C,
                                 const Level         L,
                                 const AddressBook * const wanders);


            //! generate with components only
            /**
             - compute when best.size>0
             \param xadd    for additions
             \param Csub    SubLevel concenrtations, initialized to C
             \param E       equilibrium
             \param C       original concentrations
             \param L       original level
             \param wanders for testing wandering species
             \return positive or zero gain
             */
            xreal_t generate(XAdd             &xadd,
                             XWritable        &Csub,
                             const Components &E,
                             const XReadable  &C,
                             const Level       L,
                             const AddressBook * const wanders) const;


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

