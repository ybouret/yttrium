
//! \file

#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/types.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //! boundary extent and vanishing species
        class Boundary : public SRepo
        {
        public:

            //! setup empty
            explicit Boundary(const SBank &) noexcept;

            //! setup with initial value/speice
            explicit Boundary(const SBank   & b,
                              const xreal_t   x,
                              const Species & s);

            //! duplicate
            explicit Boundary(const Boundary &);
            
            //! cleanup
            virtual ~Boundary() noexcept {}


            Y_OSTREAM_PROTO(Boundary);

            //! first/update x>=0
            void operator()(const xreal_t x,
                            const Species &s);

            //! free/xi=0
            void empty() noexcept;

            //! set first (must be empty)
            void first(const xreal_t x, const Species &s);

            
            xreal_t xi;

        private:
            Y_DISABLE_ASSIGN(Boundary);




        };
    }

}

#endif

