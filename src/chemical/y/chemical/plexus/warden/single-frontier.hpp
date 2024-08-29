
//! \file

#ifndef Y_Chemical_Warden_SingleFrontier_Included
#define Y_Chemical_Warden_SingleFrontier_Included 1

#include "y/chemical/plexus/warden/sproxy.hpp"
#include "y/chemical/types.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! single frontier to find limiting extent to a transformation
        //
        //
        //______________________________________________________________________
        class SingleFrontier : public SProxy, public Recyclable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SingleFrontier(const SBank &) noexcept;
            virtual ~SingleFrontier()              noexcept;
            Y_OSTREAM_PROTO(SingleFrontier);

           

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! [Recyclable] clean list and xi=0
            virtual void  free() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            void operator()(const xreal_t  x, const Species &s);//! initialize/update with x>=0
            bool blocking() const noexcept;                     //!< blocking if has species with zero xi


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const xreal_t xi; //!< limiting extent for vanishing species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SingleFrontier);
            void first(const xreal_t x, const Species &s);
        };

    }

}


#endif

