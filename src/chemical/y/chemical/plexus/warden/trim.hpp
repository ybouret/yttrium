
//! \file

#ifndef Y_Chemical_Warden_Trim_Included
#define Y_Chemical_Warden_Trim_Included 1

#include "y/chemical/plexus/warden/single-frontier.hpp"
#include "y/chemical/plexus/warden/frontiers.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! limiting and required for ONE side of an equilibrium
        //
        //
        //______________________________________________________________________
        class Trim : public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Trim(const Fund &) noexcept;
            virtual ~Trim()             noexcept;
            Y_OSTREAM_PROTO(Trim);

           

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

            //! compute from concentrations
            /**
             \param C concentrations
             \param L level
             \param A actors: reac or prod of an Equilibrium
             \param conserved database of conserved species
             */
            bool operator()(const XReadable   &C,
                            const Level        L,
                            const Actors      &A,
                            const AddressBook &conserved);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SingleFrontier  limiting; //!< from positive or zero concentrations
            Frontiers       required; //!< from negative concentrations

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Trim);
        };
    }

}

#endif

