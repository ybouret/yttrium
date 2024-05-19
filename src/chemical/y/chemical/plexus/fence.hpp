
//! \file

#ifndef Y_Chemical_Fence_Included
#define Y_Chemical_Fence_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/plexus/banks.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {



        //______________________________________________________________________
        //
        //
        //
        //! Fence made of boundaries for a controller
        //
        //
        //______________________________________________________________________
        class Fence
        {
        public:
            static const unsigned RUNNING = 0x01;                        //!< no negative
            static const unsigned BLOCKED = 0x02;                        //!< blocked
            static const unsigned PARTIAL = 0x04;                        //!< partially solved
            static const unsigned EQUATED = 0x08;                        //!< totally solved
            static const unsigned DISCARD = RUNNING | BLOCKED;           //!< cannot move
            static const unsigned IMPROVE = PARTIAL | EQUATED;           //!< can improve
            static const unsigned ST_MASK = DISCARD | IMPROVE;           //!< status mask
            static const unsigned BY_REAC = 0x10;                        //!< by use of reac
            static const unsigned BY_PROD = 0x20;                        //!< by use of prod
            static const unsigned BY_BOTH = BY_REAC | BY_PROD;           //!< alias
            static const unsigned ID_MASK = BY_REAC | BY_PROD | BY_BOTH; //!< alias

            static const char * StatusText(const unsigned) noexcept; //!< RUNNING|BLOCKED|PARTIAL|EQUATED

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Fence(Banks &) noexcept ; //!< setup
            Fence(const Fence &);     //!< copy
            ~Fence() noexcept;        //!< cleanup
            Y_OSTREAM_PROTO(Fence);   //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! reset all limits
            void reset() noexcept;


            //! compute cursor for controller (reac+prod)
            /**
             build limits then study what is possible
             \param components controller's components, a.k.a conserved species
             \param Ctop       top-level concentrations
             \param xml        verbosity
             \return a matching flag
             */
            unsigned operator()(const Components &components,
                                const XReadable  &Ctop,
                                XMLog            &xml);



            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Limits   capping; //!< capping by positive concentrations
            const Limits   missing; //!< missing by negative concentrations
            const xreal_t  cursor;  //!< strategy when possible
            const SRepo    zeroed;  //!< vanishing when possible

        private:
            Y_DISABLE_ASSIGN(Fence);

            void     startUpWith(const Boundary &bnd, const Equilibrium::Direction dir);
            void     proceedWith(const Boundary &bnd); //!< append species to be zeroed
            unsigned missingProd(XMLog &xml);
            unsigned missingReac(XMLog &xml);
            void     buildLimits(const Components &, const XReadable &);
        };

    }

}

#endif

