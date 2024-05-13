
//! \file

#ifndef Y_Chemical_Limits_Included
#define Y_Chemical_Limits_Included 1

#include "y/chemical/reactive/plexus/boundaries.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Limits are boundaries for reactants and products
        //
        //
        //______________________________________________________________________
        class Limits
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            static const unsigned USE_NONE = 0x00;                  //!< alias for state
            static const unsigned USE_REAC = 0x01;                  //!< alias for state
            static const unsigned USE_PROD = 0x02;                  //!< alias for state
            static const unsigned USE_BOTH = USE_REAC | USE_PROD;   //!< alias for state

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Limits(const BBank &, const SBank &) noexcept; //!< setup
            Limits(const Limits &);                                 //!< copy
            ~Limits() noexcept;                                     //!< clenaup
            Y_OSTREAM_PROTO(Limits);                                //!< display

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            void     reset()       noexcept; //!< reset all boundaries
            unsigned state() const noexcept; //!< current state w.r.t to

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Boundaries reac; //!< boundaries for reactants
            Boundaries prod; //!< boundaries for products
        private:
            Y_DISABLE_ASSIGN(Limits);
            void outReac(std::ostream &os) const;
            void outProd(std::ostream &os) const;

        };

    }

}

#endif
