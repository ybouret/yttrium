
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
            explicit Trims(const Fund &fund) noexcept :
            reac(fund),
            prod(fund)
            {
            }

            virtual ~Trims() noexcept
            {
            }

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! [Recyclable] free all
            virtual void free() noexcept
            {
                reac.free();
                prod.free();
            }

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
                            const AddressBook &conserved)
            {
                free();
                try {

                    if(reac(C,L,E.reac,conserved))
                    {
                        //------------------------------------------------------
                        assert(reac.required.size>0);
                        //------------------------------------------------------
                        if(prod(C,L,E.prod,conserved))
                        {
                            //--------------------------------------------------
                            assert(reac.required.size>0);
                            assert(prod.required.size>0);
                            //--------------------------------------------------
                            return BadBoth;
                        }
                        else
                        {
                            //--------------------------------------------------
                            assert(reac.required.size>0);
                            assert(prod.required.size<=0);
                            //--------------------------------------------------
                            return BadReac;
                        }

                    }
                    else
                    {
                        //------------------------------------------------------
                        assert(reac.required.size<=0);
                        //------------------------------------------------------
                        if(prod(C,L,E.prod,conserved))
                        {
                            //--------------------------------------------------
                            assert(reac.required.size<=0);
                            assert(prod.required.size>0);
                            //--------------------------------------------------
                            return BadProd;
                        }
                        else
                        {
                            //--------------------------------------------------
                            assert(reac.required.size<=0);
                            assert(prod.required.size<=0);
                            //--------------------------------------------------
                            return BadNone;
                        }
                    }
                } catch(...) {
                    free(); throw;
                }
            }


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

