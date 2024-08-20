//! \file

#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/types.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! boundary extent and corresponding vanishing species
        //
        //
        //______________________________________________________________________
        class Boundary : public SRepo
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with initial value/speice
            explicit Boundary(const SBank   & b,
                              const xreal_t   x,
                              const Species & s);

            explicit Boundary(const SBank &) noexcept; //!< setup empty
            explicit Boundary(const Boundary &);       //!< duplicate
            virtual ~Boundary() noexcept;               //!< cleanup
            Y_OSTREAM_PROTO(Boundary);                 //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
           
            //! set first or update x>=0
            void operator()(const xreal_t x,
                            const Species &s);

            //! helper: free/xi=0
            void empty() noexcept;

            //! helper: set first (must be empty)
            void first(const xreal_t x, const Species &s);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            xreal_t xi; //!< extent to which species are vanishing

        private:
            Y_DISABLE_ASSIGN(Boundary);




        };
    }

}

#endif

