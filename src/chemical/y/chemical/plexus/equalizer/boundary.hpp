
//! \file


#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/plexus/equalizer/restartable.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! extent shared for (multiple) species
        //
        //
        //______________________________________________________________________
        class Boundary : public SRepo, public Restartable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Boundary(const SBank &) noexcept; //!< setup empty
            virtual ~Boundary()              noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Boundary);                 //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! process species with its extent
            void operator()(const Species &sp, const xreal_t xx);

            //! free and xi=0
            virtual void restart() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            xreal_t xi; //!< current extent
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundary);
        };



    }

}


#endif
