
//! \file

#ifndef Y_Chemical_Fence_Included
#define Y_Chemical_Fence_Included 1

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/chemical/plexus/limits.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Banks
        {
        public:
            Banks() : hb(), ls()  {}
            ~Banks() {}
            BBank hb; //!< heavy boundaries
            SBank ls; //!< light species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Banks);
        };

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

            //! shape limits from components and concentrations
            Fence & shapeFull(const Components &components,
                              const XReadable  &Ctop);

            //! compute cursor for controller
            unsigned studyFull(XMLog &xml);



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
            const xreal_t  zero;    //!< alias
            
        private:
            Y_DISABLE_ASSIGN(Fence);

            void     startUpWith(const Boundary &bnd, const Equilibrium::Direction dir);
            void     proceedWith(const Boundary &bnd); //!< append species to be zeroed
            unsigned missingProd(XMLog &xml);
            unsigned missingReac(XMLog &xml);
        };

    }

}

#endif
