
//! \file


#ifndef Y_Chemical_Initial_Design_Included
#define Y_Chemical_Initial_Design_Included 1

#include "y/chemical/plexus/initial/axiom.hpp"
#include "y/chemical/plexus/initial/axioms.hpp"
#include "y/chemical/library.hpp"
#include "y/chemical/plexus/clusters.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            //__________________________________________________________________
            //
            //
            //
            //! Design an initial concentration from prescribed Axioms
            //
            //
            //__________________________________________________________________
            class Design :
            public Quantized,
            public Entity,
            public Proxy< ListOf<Axiom> >,
            public Latchable
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const          CallSign;              //!< "Initial::Design"
                typedef Proxy< ListOf<Axiom> >     BaseType;              //!< alias
                typedef ArkPtr<String,Design>      Pointer;               //!< alias
                static const char * const          Logo;                  //!< "Logo"
                static const char * const          LogoExpr;              //!< "[.][:word:]+"
                static const char * const          _FixedConcentration;   //!< "FixedConcentration"
                static const char * const          _SteadyConservation;   //!< "SteadyConservation"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with identifier
                template <typename UUID>
                explicit Design(const UUID &uuid) :
                Entity( new String(uuid) ),
                BaseType(),
                Latchable(),
                my()
                {
                }

                //! duplicate a design with a new identfier
                template <typename UUID>
                explicit Design(const UUID &uuid, const Design &root) :
                Entity( new String(uuid) ),
                BaseType(),
                Latchable(),
                my(root.my)
                {
                }

                //! cleanup
                virtual ~Design() noexcept;

                //! setup from pre-compiled axioms
                explicit Design(const Axioms   &axioms,
                                const Library  &lib,
                                const Clusters &cls);

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void add(Axiom * const); //!< add a new Axiom, mut be unlatched

                //! add a new species conservation
                void add(Clusters     &clusters,
                         const String &expr,
                         const xreal_t Ctot);

                //! build
                void build(XMLog          &xml,
                           XWritable      &C0,
                           const Library  &lib,
                           const Clusters &cls);

                //! build
                void build(XMLog         &xml,
                           XWritable     &Cs,
                           XMatrix       &Qr,
                           const Library &lib,
                           const Clusters &cls);


                //! check if fixed concentration is EXPLICITLY set for species
                bool defines(const Species &sp) const noexcept;



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Design);
                Y_PROXY_DECL();
                ListOfCloneable<Axiom> my;

                //! check if a species it not defined, set it to zero
                bool foundZeroConcentration(XMLog &xml, const Clusters &cls);

                //! check if a species of the list is defined
                /**
                 - if not explicitly defined, set it to zero
                 */
                bool foundZeroConcentrationIn(XMLog &, const SList &);
            };



        }

    }

}

#endif

