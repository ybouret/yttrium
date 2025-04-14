//! \file

#ifndef Y_Chemical_Extents_Included
#define Y_Chemical_Extents_Included 1

#include "y/chemical/plexus/equalizing/extent.hpp"
#include "y/chemical/reactive/components.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {
            //__________________________________________________________________
            //
            //
            //! Extents analyze result
            //
            //__________________________________________________________________
            enum Resultant
            {
                Correct, //!< balanced
                BadReac, //!< at least one bad reactant, products are ok
                BadProd, //!< at least one bad product, reactants are ok
                BadBoth  //!< at least one bad on each side
            };
            
            
            //! Human Readable Resultant
            const char * ResultantText(const Resultant) noexcept;
            
            //__________________________________________________________________
            //
            //
            //
            //! classified extents for both sides of an equilibrium
            //
            //
            //__________________________________________________________________
            class Extents : public Restartable
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Extents(const Banks &banks) noexcept; //!< setup empty
                virtual ~Extents() noexcept;                   //!< cleanup
                
                
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! restart all
                virtual void restart() noexcept;
                
                //! dispatch all
                Resultant operator()(XMLog &             xml,
                                     const Components &  E,
                                     const XReadable &   C,
                                     const Level         L,
                                     const AddressBook & wanders);
                
                
                //! generate with components only
                /**
                 - compute when best.size>0
                 \param xml     for output
                 \param xadd    for additions
                 \param Csub    SubLevel concentrations, initialized to C
                 \param E       equilibrium
                 \param C       original concentrations
                 \param L       original level
                 \param wanders for testing wandering species
                 \return positive or zero gain
                 */
                xreal_t generate(XMLog            &xml,
                                 XAdd             &xadd,
                                 XWritable        &Csub,
                                 const Components &E,
                                 const XReadable  &C,
                                 const Level       L,
                                 const AddressBook &wanders) const;
                
                
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Extent   reac; //!< status for reactant(s)
                Extent   prod; //!< status for product(s)
                Boundary best; //!< best effort
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Extents);
                void findBest(XMLog &xml,
                              const Boundary &limiting,
                              const Cursors  &requried);
            };
            
        }

    }

}

#endif

