
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
        class Boundary : public SRepo, public Restartable
        {
        public:
            explicit Boundary(const SBank &) noexcept;
            virtual ~Boundary()              noexcept;
            Y_OSTREAM_PROTO(Boundary);

            //! process species with its extent
            void operator()(const Species &sp, const xreal_t xx);

            //! free and xi=0
            virtual void restart() noexcept;

            xreal_t xi;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boundary);
        };



    }

}


#endif
