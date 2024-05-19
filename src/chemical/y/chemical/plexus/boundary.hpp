
//! \file

#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Boundary = extent with one or more species
        //
        //
        //______________________________________________________________________
        class Boundary : public Proxy<const SRepo>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! record first species with its extent
            explicit Boundary(const Species &s,
                              const xreal_t  x,
                              const SBank   &bank);
            virtual ~Boundary() noexcept;    //!< cleanup
            Boundary(const Boundary &other); //!< full copy
            Y_OSTREAM_PROTO(Boundary);       //!< display list+xi

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Boundary & operator<<(const Species &); //!< append another species

            //__________________________________________________________________
            //
            //
            //  Members
            //
            //__________________________________________________________________
            const xreal_t xi; //!< extent

        private:
            Y_DISABLE_ASSIGN(Boundary);
            virtual ConstInterface & surrogate() const noexcept;
            SRepo sr;
        };



    }

}

#endif
